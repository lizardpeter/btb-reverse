#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <commdlg.h>

#include "btb/game_session.hpp"
#include "btb/application_state.hpp"

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

namespace {
using btb::dino::Vec2i;
namespace fs = std::filesystem;

constexpr int kWidth = static_cast<int>(btb::application::kRetailWidth);
constexpr int kHeight = static_cast<int>(btb::application::kRetailHeight);
constexpr UINT_PTR kFrameTimer = 1;

struct Bitmap {
    HBITMAP handle{};
    int width{};
    int height{};

    void release() noexcept {
        if (handle) {
            DeleteObject(handle);
        }
        handle = nullptr;
        width = height = 0;
    }

    void load(const fs::path& file) {
        release();
        handle = static_cast<HBITMAP>(LoadImageW(
            nullptr, file.c_str(), IMAGE_BITMAP, 0, 0,
            LR_LOADFROMFILE | LR_CREATEDIBSECTION));
        if (handle) {
            BITMAP data{};
            if (GetObjectW(handle, sizeof(data), &data) == sizeof(data)) {
                width = data.bmWidth;
                height = data.bmHeight;
            } else {
                release();
            }
        }
    }
};

void text_at(HDC dc, int x, int y, const std::string& value) {
    TextOutA(dc, x, y, value.c_str(), static_cast<int>(value.size()));
}

void box(HDC dc, int x, int y, int w, int h, COLORREF color) {
    RECT rect{x, y, x + w, y + h};
    HBRUSH brush = CreateSolidBrush(color);
    FillRect(dc, &rect, brush);
    DeleteObject(brush);
}

void blit(HDC dc, const Bitmap& image, int x, int y, bool keyed) {
    if (!image.handle || image.width <= 0 || image.height <= 0) {
        return;
    }
    HDC src = CreateCompatibleDC(dc);
    if (!src) {
        return;
    }
    const HGDIOBJ old = SelectObject(src, image.handle);
    if (keyed) {
        TransparentBlt(dc, x, y, image.width, image.height,
                       src, 0, 0, image.width, image.height,
                       RGB(255, 0, 255));
    } else {
        BitBlt(dc, x, y, image.width, image.height,
               src, 0, 0, SRCCOPY);
    }
    SelectObject(src, old);
    DeleteDC(src);
}

class Host {
public:
    HWND hwnd{};
    btb::host::Session game{};
    int species{};
    int difficulty{};
    Vec2i mouse{320, 240};
    std::vector<Bitmap> pieces{};
    Bitmap background{};
    Bitmap certificate{};

    ~Host() {
        clear_bitmaps();
    }

    void clear_bitmaps() {
        for (auto& s : pieces) {
            s.release();
        }
        pieces.clear();
        background.release();
        certificate.release();
    }

    Vec2i client_point(LPARAM lp) const noexcept {
        RECT client{};
        GetClientRect(hwnd, &client);
        const int width = std::max(1L, client.right - client.left);
        const int height = std::max(1L, client.bottom - client.top);
        const int raw_x = static_cast<short>(LOWORD(lp));
        const int raw_y = static_cast<short>(HIWORD(lp));
        return {
            std::clamp(raw_x * kWidth / width, 0, kWidth - 1),
            std::clamp(raw_y * kHeight / height, 0, kHeight - 1),
        };
    }

    void load_artwork() {
        clear_bitmaps();
        const auto* dino = game.dinosaur();
        if (!dino) {
            return;
        }
        const auto directory = game.level_path().parent_path();
        pieces.resize(dino->pieces().size());
        for (std::size_t i = 0; i < pieces.size(); ++i) {
            const int id = dino->pieces()[i].piece_id;
            pieces[i].load(directory /
                           ("piece" + std::to_string(id) + ".bmp"));
            // Fallback is explicitly illustrative, never described as retail
            // geometry. The playable host requires actual BMP dimensions for
            // pixel-exact loose-piece hit testing.
            game.set_piece_dimensions(
                i,
                pieces[i].handle ? pieces[i].width : 44,
                pieces[i].handle ? pieces[i].height : 44);
        }

        // Optional artwork probes; neither name is asserted to be the exact
        // retail background path for every Dinosaur level.
        background.load(directory / "background.bmp");
        if (!background.handle) {
            background.load(directory / "bg.bmp");
        }

        const auto art_name = fs::path(
            btb::dino::completion_artwork_filename(game.species()))
            .filename();
        auto search = directory;
        for (int depth = 0; depth < 6; ++depth) {
            certificate.load(search / art_name);
            if (certificate.handle) {
                break;
            }
            certificate.load(search / "data" / "subgamedino" / art_name);
            if (certificate.handle) {
                break;
            }
            if (!search.has_parent_path() || search == search.parent_path()) {
                break;
            }
            search = search.parent_path();
        }
        game.tick();
    }

    void choose_level() {
        wchar_t filename[32768]{};
        OPENFILENAMEW dialog{};
        dialog.lStructSize = sizeof(dialog);
        dialog.hwndOwner = hwnd;
        dialog.lpstrFilter = L"Dinosaur level (dino.txt)\0dino.txt\0All files\0*.*\0\0";
        dialog.lpstrFile = filename;
        dialog.nMaxFile = static_cast<DWORD>(sizeof(filename) / sizeof(filename[0]));
        dialog.lpstrTitle = L"Choose original Dinosaur dino.txt";
        dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (!GetOpenFileNameW(&dialog)) {
            return;
        }

        if (game.load_dinosaur(
                fs::path(filename),
                static_cast<btb::dino::Species>(species),
                static_cast<btb::dino::Difficulty>(difficulty))) {
            load_artwork();
        } else {
            MessageBoxA(hwnd, game.error().c_str(),
                        "Unable to load Dinosaur level", MB_OK | MB_ICONERROR);
        }
        InvalidateRect(hwnd, nullptr, FALSE);
    }

    void return_to_menu() {
        clear_bitmaps();
        game.back_to_activity_select();
        InvalidateRect(hwnd, nullptr, FALSE);
    }

    void draw_piece(HDC dc, std::size_t slot, int x, int y) {
        if (!game.dinosaur() || slot >= pieces.size()) {
            return;
        }
        const auto& sprite = pieces[slot];
        if (sprite.handle) {
            blit(dc, sprite, x, y, true);
            return;
        }

        // Visible non-retail placeholder for missing source art.
        const auto& piece = game.dinosaur()->pieces()[slot];
        box(dc, x, y, 44, 44, RGB(222, 183, 115));
        SetTextColor(dc, RGB(34, 30, 21));
        text_at(dc, x + 5, y + 13,
                "#" + std::to_string(piece.piece_id));
    }

    void paint_game(HDC dc) {
        box(dc, 0, 0, kWidth, kHeight, RGB(236, 226, 195));
        if (background.handle) {
            blit(dc, background, 0, 0, false);
        }

        const auto* dino = game.dinosaur();
        if (!dino) {
            return;
        }
        // With missing original background art, draw visible target guides;
        // these are deliberately NOT a claim of reconstructed retail visuals.
        if (!background.handle) {
            HPEN pen = CreatePen(PS_DOT, 2, RGB(82, 114, 107));
            const HGDIOBJ prev_pen = SelectObject(dc, pen);
            const HGDIOBJ prev_brush = SelectObject(
                dc, GetStockObject(HOLLOW_BRUSH));
            for (const auto& piece : dino->pieces()) {
                Ellipse(dc, piece.target.x - 10, piece.target.y - 10,
                        piece.target.x + 10, piece.target.y + 10);
            }
            SelectObject(dc, prev_brush);
            SelectObject(dc, prev_pen);
            DeleteObject(pen);
        }

        // The composition and its layer ordering come from the recovered
        // retail DrawDinoActivity reconstruction.
        for (const auto& cmd : game.dino_frame().commands) {
            if ((cmd.layer == btb::dino::DrawLayer::PlacedPiece ||
                 cmd.layer == btb::dino::DrawLayer::LoosePiece ||
                 cmd.layer == btb::dino::DrawLayer::SpecialPiece) &&
                cmd.piece_index) {
                draw_piece(dc, *cmd.piece_index, cmd.x, cmd.y);
            }
        }
        if (const auto carried = game.carried_piece()) {
            const auto p = game.carried_piece_top_left(mouse);
            draw_piece(dc, *carried, p.x, p.y);
        }

        box(dc, 0, 0, kWidth, 36, RGB(38, 60, 61));
        SetTextColor(dc, RGB(255, 255, 255));
        text_at(dc, 12, 11, "Dinosaur  |  retail puzzle logic / C++26 host");
        box(dc, 533, 5, 97, 26, RGB(190, 216, 206));
        SetTextColor(dc, RGB(23, 41, 45));
        text_at(dc, 548, 11, "Back [Esc]");
        SetTextColor(dc, RGB(37, 52, 49));
        box(dc, 8, 444, kWidth - 16, 29, RGB(221, 231, 217));
        text_at(dc, 18, 452,
                "Placed: " +
                std::to_string(dino->completed_piece_count()) + " / " +
                std::to_string(dino->pieces().size()) +
                "    Click and drag bones to their target.");
        if (game.screen() == btb::host::Screen::DinosaurComplete) {
            box(dc, 70, 170, 500, 126, RGB(214, 234, 207));
            SetTextColor(dc, RGB(29, 72, 47));
            text_at(dc, 123, 195, "Dinosaur complete!");
            text_at(dc, 97, 231,
                    "Retail certificate/audio integration pending.");
            text_at(dc, 141, 261, "Press Escape to choose another level.");
        }
    }

    void paint_menu(HDC dc) {
        box(dc, 0, 0, kWidth, kHeight, RGB(233, 238, 232));
        box(dc, 0, 0, kWidth, 83, RGB(36, 61, 63));
        SetTextColor(dc, RGB(248, 248, 234));
        text_at(dc, 38, 23, "BOB BUILDS A PARK");
        text_at(dc, 38, 49, "Source reconstruction | First integrated C++26 build");
        SetTextColor(dc, RGB(39, 65, 67));
        text_at(dc, 38, 118, "Dinosaur skeleton puzzle  -  playable");
        text_at(dc, 38, 144, "Open dino.txt from your installed original game.");
        box(dc, 38, 188, 564, 56, RGB(177, 209, 196));
        SetTextColor(dc, RGB(22, 48, 47));
        text_at(dc, 58, 207, "OPEN ORIGINAL DINO LEVEL  [O]");
        static constexpr const char* names[]{"Raptor", "Triceratops", "T-Rex"};
        static constexpr const char* difficulties[]{"Easy", "Medium", "Hard"};
        text_at(dc, 38, 275,
                "Species [1/2/3]: " + std::string(names[species]));
        text_at(dc, 38, 303,
                "Difficulty [Q/W/E]: " + std::string(difficulties[difficulty]));
        text_at(dc, 38, 365,
                "Other minigames: source modules present, not yet hosted.");
        text_at(dc, 38, 391,
                "Rendering: source BMPs when present; missing art marked.");
        text_at(dc, 38, 420,
                "Not a complete, visually faithful retail game build.");
    }

    void paint(HDC output) {
        RECT bounds{};
        GetClientRect(hwnd, &bounds);
        HDC buffer = CreateCompatibleDC(output);
        HBITMAP backing = CreateCompatibleBitmap(output, kWidth, kHeight);
        if (!buffer || !backing) {
            if (backing) DeleteObject(backing);
            if (buffer) DeleteDC(buffer);
            return;
        }
        const HGDIOBJ old = SelectObject(buffer, backing);
        SetBkMode(buffer, TRANSPARENT);
        HFONT font = CreateFontA(
            18, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI");
        const HGDIOBJ previous_font = SelectObject(buffer, font);
        if (game.screen() == btb::host::Screen::ActivitySelect) {
            paint_menu(buffer);
        } else {
            paint_game(buffer);
        }
        SelectObject(buffer, previous_font);
        DeleteObject(font);

        SetStretchBltMode(output, COLORONCOLOR);
        StretchBlt(output, 0, 0,
                   std::max(1L, bounds.right - bounds.left),
                   std::max(1L, bounds.bottom - bounds.top),
                   buffer, 0, 0, kWidth, kHeight, SRCCOPY);
        SelectObject(buffer, old);
        DeleteObject(backing);
        DeleteDC(buffer);
    }

    LRESULT handle(UINT message, WPARAM wp, LPARAM lp) {
        switch (message) {
        case WM_CREATE:
            SetTimer(hwnd, kFrameTimer, 16, nullptr);
            return 0;
        case WM_SIZE:
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_ERASEBKGND:
            return 1;
        case WM_TIMER:
            if (wp == kFrameTimer) {
                game.tick();
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        case WM_MOUSEMOVE:
            mouse = client_point(lp);
            if (game.carried_piece()) {
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        case WM_LBUTTONDOWN:
            mouse = client_point(lp);
            if (game.screen() == btb::host::Screen::ActivitySelect) {
                if (mouse.x >= 38 && mouse.x < 602 &&
                    mouse.y >= 188 && mouse.y < 244) {
                    choose_level();
                }
            } else if (mouse.x >= 533 && mouse.y < 36) {
                return_to_menu();
            } else {
                SetCapture(hwnd);
                game.pointer_down(mouse);
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_LBUTTONUP:
            mouse = client_point(lp);
            if (GetCapture() == hwnd) {
                ReleaseCapture();
                game.pointer_up(mouse);
            } else if (game.carried_piece()) {
                game.pointer_up(mouse);
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_KEYDOWN:
            if (wp == VK_ESCAPE) {
                if (game.screen() == btb::host::Screen::ActivitySelect) {
                    DestroyWindow(hwnd);
                } else {
                    return_to_menu();
                }
            } else if (wp == 'O') {
                choose_level();
            } else if (game.screen() == btb::host::Screen::ActivitySelect) {
                if (wp >= '1' && wp <= '3') species = static_cast<int>(wp - '1');
                if (wp == 'Q') difficulty = 0;
                if (wp == 'W') difficulty = 1;
                if (wp == 'E') difficulty = 2;
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps{};
            HDC dc = BeginPaint(hwnd, &ps);
            paint(dc);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            KillTimer(hwnd, kFrameTimer);
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hwnd, message, wp, lp);
        }
    }
};

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp) {
    if (message == WM_NCCREATE) {
        auto* init = reinterpret_cast<CREATESTRUCTW*>(lp);
        auto* host = static_cast<Host*>(init->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA,
                          reinterpret_cast<LONG_PTR>(host));
        host->hwnd = hwnd;
    }
    auto* host = reinterpret_cast<Host*>(
        GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (host) {
        return host->handle(message, wp, lp);
    }
    return DefWindowProcW(hwnd, message, wp, lp);
}
} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    Host host;
    WNDCLASSEXW cls{};
    cls.cbSize = sizeof(cls);
    cls.style = CS_HREDRAW | CS_VREDRAW;
    cls.lpfnWndProc = window_proc;
    cls.hInstance = instance;
    cls.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    cls.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    cls.lpszClassName = L"BTBSourceReconstruction";
    if (!RegisterClassExW(&cls)) {
        return 1;
    }

    RECT bounds{0, 0, kWidth, kHeight};
    AdjustWindowRect(&bounds, WS_OVERLAPPEDWINDOW, FALSE);
    HWND window = CreateWindowExW(
        0, cls.lpszClassName,
        L"Bob the Builder: Bob Builds a Park - C++26 Reconstruction",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        bounds.right - bounds.left, bounds.bottom - bounds.top,
        nullptr, nullptr, instance, &host);
    if (!window) {
        return 2;
    }
    ShowWindow(window, show);
    UpdateWindow(window);
    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return static_cast<int>(message.wParam);
}

#endif
