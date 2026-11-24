# Legacy CMake entrypoint

The faithful source, runtime, headers and tests now live in the repository root. `cmake -S reconstruction -B build/reconstruction` remains supported and includes the exact same shared build graph as `cmake -S .`. See [source layout](../docs/source-layout.md).
