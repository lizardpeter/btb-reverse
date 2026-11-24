#include "batch20_services.hpp"
#include "batch20_vectors.hpp"
#include <iostream>
using namespace btb::original_printer7;
using namespace batch20;
int main(int argc,char** argv){
 if(argc!=2 || !verify_7_original_printer_functions())return 1;std::ifstream file(argv[1]);if(!file)return 2;std::string line,kind;unsigned checked{};
 while(std::getline(file,line)) {
  std::istringstream in(line);in>>kind;if(kind!="PRINTER")continue;
  const auto mode=word(in);PrinterServices io;io.width=integer(in);io.height=integer(in);io.bits=word(in);io.colors=word(in);io.section=word(in)!=0;io.doc_result=integer(in);io.page_result=integer(in);const U32 palette=word(in),offset=word(in);const auto expected=bytes(in);auto expected_trace=trace(in);
  Printer p;p.target={0,0,640,480};std::array<std::byte,40> header{};
  if(mode>=3){const std::array<U32,10> h{40,static_cast<U32>(io.width),static_cast<U32>(io.height),1U|(24U<<16),0,0,0,0,0,0};std::memcpy(header.data(),h.data(),40);p.dib=header.data();p.bits=header.data()+40;}
  if(mode==0)import_ddb(p,1,palette,io);else if(mode==1)import_section(p,1,io);else if(mode==2)set_bitmap(p,1,palette,io);else if(mode==3)render_to_dc(p,1,io);else if(mode==4)print_document(p,1,io);else stretch_dib(p,1,io);
  if(!in || static_cast<U32>(p.bits-p.dib)!=offset || std::string(reinterpret_cast<char*>(p.dib),expected.size())!=expected || io.trace!=expected_trace){std::cerr<<"Printer reference mismatch "<<checked<<": "<<line<<'\n';return 3;}++checked;
 }
 assert(checked==240);
 auto expect_error=[](const char* text,auto fn){bool caught=false;try{fn();}catch(const Error& e){caught=std::string(e.what())==text && e.retail_object().base.vtable==0x43b328 && std::string(e.retail_object().text.data)==text;}assert(caught);};
 PrinterServices io;Printer p;
 expect_error("No bitmap defined.",[&]{render_to_dc(p,0,io);});
 io.bitmap_result=0;expect_error("Invalid bitmap.",[&]{set_bitmap(p,1,0,io);});
 io.bitmap_result=24;import_ddb(p,1,0,io);
 expect_error("Invalid printer DC.",[&]{print_document(p,0,io);});
 p.target={};expect_error("Invalid target rectangle.",[&]{stretch_dib(p,1,io);});
 p.target={0,0,640,480};io.caps=0x100;expect_error("Unsupported printer.",[&]{stretch_dib(p,1,io);});
 io.caps=0;io.stretch_result=-1;io.trace.clear();expect_error("Error printing DIB.",[&]{print_document(p,1,io);});
 assert(io.trace.size()>2 && io.trace[io.trace.size()-2]==22 && io.trace.back()==23);
 io.stretch_result=1;io.section_result=0;expect_error("Error getting DIB section info.",[&]{import_section(p,1,io);});
 io.section_result=84;io.compression=3;io.bits=24;io.trace.clear();import_section(p,1,io);assert(io.trace[0]==2 && io.trace[1]==1);
 io.dibits_result=0;expect_error("Error getting DDB bits.",[&]{import_ddb(p,1,0,io);});
 std::cout<<checked<<" original GDI request/data comparisons and printing failure/cleanup tests passed\n";
}
