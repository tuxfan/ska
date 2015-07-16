; ModuleID = 'residual.cc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%"struct.boost::arg" = type { i8 }
%"struct.boost::arg.0" = type { i8 }
%"struct.boost::arg.2" = type { i8 }
%"struct.boost::arg.4" = type { i8 }
%"struct.boost::arg.6" = type { i8 }
%"struct.boost::arg.8" = type { i8 }
%"struct.boost::arg.10" = type { i8 }
%"struct.boost::arg.12" = type { i8 }
%"struct.boost::arg.14" = type { i8 }
%"struct.boost::spirit::classic::nothing_parser" = type { i8 }
%"struct.boost::spirit::classic::anychar_parser" = type { i8 }
%"struct.boost::spirit::classic::alnum_parser" = type { i8 }
%"struct.boost::spirit::classic::alpha_parser" = type { i8 }
%"struct.boost::spirit::classic::cntrl_parser" = type { i8 }
%"struct.boost::spirit::classic::digit_parser" = type { i8 }
%"struct.boost::spirit::classic::graph_parser" = type { i8 }
%"struct.boost::spirit::classic::lower_parser" = type { i8 }
%"struct.boost::spirit::classic::print_parser" = type { i8 }
%"struct.boost::spirit::classic::punct_parser" = type { i8 }
%"struct.boost::spirit::classic::blank_parser" = type { i8 }
%"struct.boost::spirit::classic::space_parser" = type { i8 }
%"struct.boost::spirit::classic::upper_parser" = type { i8 }
%"struct.boost::spirit::classic::xdigit_parser" = type { i8 }
%"struct.boost::spirit::classic::eol_parser" = type { i8 }
%"struct.boost::spirit::classic::end_parser" = type { i8 }
%"struct.boost::spirit::classic::inhibit_case_parser_gen" = type { i8 }
%"struct.boost::spirit::classic::sign_parser" = type { i8 }
%"struct.boost::spirit::classic::real_parser" = type { i8 }
%"struct.boost::spirit::classic::real_parser.45" = type { i8 }
%"struct.boost::spirit::classic::real_parser.47" = type { i8 }
%"struct.boost::spirit::classic::real_parser.49" = type { i8 }
%"struct.boost::spirit::classic::epsilon_parser" = type { i8 }
%"class.boost::spirit::classic::refactor_unary_gen" = type { %"struct.boost::spirit::classic::non_nested_refactoring" }
%"struct.boost::spirit::classic::non_nested_refactoring" = type { i8 }
%"class.boost::spirit::classic::refactor_action_gen" = type { %"struct.boost::spirit::classic::non_nested_refactoring" }
%"class.boost::spirit::classic::attach_action_gen" = type { %"struct.boost::spirit::classic::non_nested_refactoring" }
%"struct.boost::spirit::classic::list_parser_gen" = type { %"struct.boost::spirit::classic::list_parser" }
%"struct.boost::spirit::classic::list_parser" = type { %"struct.boost::spirit::classic::kleene_star", %"struct.boost::spirit::classic::chlit", %"struct.boost::spirit::classic::no_list_endtoken" }
%"struct.boost::spirit::classic::kleene_star" = type { %"class.boost::spirit::classic::unary" }
%"class.boost::spirit::classic::unary" = type { %"struct.boost::spirit::classic::anychar_parser" }
%"struct.boost::spirit::classic::chlit" = type { i8 }
%"struct.boost::spirit::classic::no_list_endtoken" = type { i8 }
%"struct.phoenix::actor" = type { i8 }
%"struct.phoenix::actor.56" = type { i8 }
%"struct.phoenix::actor.58" = type { i8 }
%"struct.phoenix::actor.60" = type { i8 }
%"struct.phoenix::actor.62" = type { i8 }
%"struct.phoenix::actor.64" = type { i8 }
%"class.std::runtime_error" = type { %"class.std::exception", %"class.std::basic_string" }
%"class.std::exception" = type { i32 (...)** }
%"class.std::basic_string" = type { %"struct.std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Alloc_hider" }
%"struct.std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Alloc_hider" = type { i8* }
%"class.boost::property_tree::ptree_error" = type { %"class.std::runtime_error" }
%"class.boost::any" = type { %"class.boost::any::placeholder"* }
%"class.boost::any::placeholder" = type { i32 (...)** }
%"class.boost::property_tree::ptree_bad_data" = type { %"class.boost::property_tree::ptree_error", %"class.boost::any" }
%"class.boost::property_tree::ptree_bad_path" = type { %"class.boost::property_tree::ptree_error", %"class.boost::any" }
%"struct.boost::spirit::classic::parser" = type { i8 }
%"struct.boost::spirit::classic::char_parser" = type { i8 }
%"struct.boost::spirit::classic::char_parser.17" = type { i8 }
%"struct.boost::spirit::classic::char_parser.19" = type { i8 }
%"struct.boost::spirit::classic::char_parser.21" = type { i8 }
%"struct.boost::spirit::classic::char_parser.23" = type { i8 }
%"struct.boost::spirit::classic::char_parser.25" = type { i8 }
%"struct.boost::spirit::classic::char_parser.27" = type { i8 }
%"struct.boost::spirit::classic::char_parser.29" = type { i8 }
%"struct.boost::spirit::classic::char_parser.31" = type { i8 }
%"struct.boost::spirit::classic::char_parser.33" = type { i8 }
%"struct.boost::spirit::classic::char_parser.35" = type { i8 }
%"struct.boost::spirit::classic::char_parser.37" = type { i8 }
%"struct.boost::spirit::classic::char_parser.39" = type { i8 }
%"struct.boost::spirit::classic::parser.41" = type { i8 }
%"struct.boost::spirit::classic::parser.42" = type { i8 }
%"struct.boost::spirit::classic::inhibit_case_parser_gen_base" = type { i8 }
%"struct.boost::spirit::classic::parser.43" = type { i8 }
%"struct.boost::spirit::classic::parser.44" = type { i8 }
%"struct.boost::spirit::classic::parser.46" = type { i8 }
%"struct.boost::spirit::classic::parser.48" = type { i8 }
%"struct.boost::spirit::classic::parser.50" = type { i8 }
%"struct.boost::spirit::classic::parser.51" = type { i8 }
%"struct.boost::spirit::classic::parser.16" = type { i8 }
%"struct.phoenix::argument" = type { i8 }
%"struct.phoenix::argument.57" = type { i8 }
%"struct.phoenix::argument.59" = type { i8 }
%"struct.phoenix::argument.61" = type { i8 }
%"struct.phoenix::argument.63" = type { i8 }
%"struct.phoenix::argument.65" = type { i8 }
%"class.boxmg::bmg2d::core::StencilOp" = type { %"class.boxmg::bmg2d::core::DiscreteOp", %"class.boxmg::bmg2d::core::GridStencil" }
%"class.boxmg::bmg2d::core::DiscreteOp" = type { i32 (...)** }
%"class.boxmg::bmg2d::core::GridStencil" = type { %"class.boxmg::bmg2d::core::Array.base", i8, i8, %"class.std::vector.68" }
%"class.boxmg::bmg2d::core::Array.base" = type <{ i32 (...)**, %"class.std::vector", %"struct.std::array", %"struct.std::array.66", %"struct.std::array.67", %"struct.std::array.67", i32 }>
%"class.std::vector" = type { %"struct.std::_Vector_base" }
%"struct.std::_Vector_base" = type { %"struct.std::_Vector_base<boxmg::bmg2d::core::Stencil, AlignAllocator<boxmg::bmg2d::core::Stencil, 16> >::_Vector_impl" }
%"struct.std::_Vector_base<boxmg::bmg2d::core::Stencil, AlignAllocator<boxmg::bmg2d::core::Stencil, 16> >::_Vector_impl" = type { %"class.boxmg::bmg2d::core::Stencil"*, %"class.boxmg::bmg2d::core::Stencil"*, %"class.boxmg::bmg2d::core::Stencil"* }
%"class.boxmg::bmg2d::core::Stencil" = type { [9 x double] }
%"struct.std::array" = type { [2 x i32] }
%"struct.std::array.66" = type { [2 x i32] }
%"struct.std::array.67" = type { [2 x %"class.boost::iterator_range"] }
%"class.boost::iterator_range" = type { %"class.boost::counting_iterator", %"class.boost::counting_iterator" }
%"class.boost::counting_iterator" = type { %"class.boost::iterator_adaptor" }
%"class.boost::iterator_adaptor" = type { i32 }
%"class.std::vector.68" = type { %"struct.std::_Vector_base.69" }
%"struct.std::_Vector_base.69" = type { %"struct.std::_Vector_base<double, AlignAllocator<double, 16> >::_Vector_impl" }
%"struct.std::_Vector_base<double, AlignAllocator<double, 16> >::_Vector_impl" = type { double*, double*, double* }
%"class.boxmg::bmg2d::core::Array.72" = type <{ i32 (...)**, %"class.std::vector.68", %"struct.std::array", %"struct.std::array.66", %"struct.std::array.67", %"struct.std::array.67", i32, [4 x i8] }>
%"class.boost::iterator_facade" = type { i8 }
%"struct.mpl_::bool_" = type { i8 }
%"struct.boost::is_convertible" = type { i8 }
%"struct.boost::spirit::classic::char_parser.54" = type { i8 }
%"struct.boost::spirit::classic::parser.52" = type { i8 }
%"struct.boost::spirit::classic::parser.53" = type { i8 }
%"class.boxmg::bmg2d::core::Array" = type <{ i32 (...)**, %"class.std::vector", %"struct.std::array", %"struct.std::array.66", %"struct.std::array.67", %"struct.std::array.67", i32, [4 x i8] }>

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external global i8
@_ZTVN5boost13property_tree14ptree_bad_dataE = linkonce_odr unnamed_addr constant [5 x i8*] [i8* null, i8* bitcast ({ i8*, i8*, i8* }* @_ZTIN5boost13property_tree14ptree_bad_dataE to i8*), i8* bitcast (void (%"class.boost::property_tree::ptree_bad_data"*)* @_ZN5boost13property_tree14ptree_bad_dataD2Ev to i8*), i8* bitcast (void (%"class.boost::property_tree::ptree_bad_data"*)* @_ZN5boost13property_tree14ptree_bad_dataD0Ev to i8*), i8* bitcast (i8* (%"class.std::runtime_error"*)* @_ZNKSt13runtime_error4whatEv to i8*)], align 8
@_ZTVN5boost13property_tree14ptree_bad_pathE = linkonce_odr unnamed_addr constant [5 x i8*] [i8* null, i8* bitcast ({ i8*, i8*, i8* }* @_ZTIN5boost13property_tree14ptree_bad_pathE to i8*), i8* bitcast (void (%"class.boost::property_tree::ptree_bad_path"*)* @_ZN5boost13property_tree14ptree_bad_pathD2Ev to i8*), i8* bitcast (void (%"class.boost::property_tree::ptree_bad_path"*)* @_ZN5boost13property_tree14ptree_bad_pathD0Ev to i8*), i8* bitcast (i8* (%"class.std::runtime_error"*)* @_ZNKSt13runtime_error4whatEv to i8*)], align 8
@_ZN12_GLOBAL__N_12_1E = internal global %"struct.boost::arg" zeroinitializer, align 1
@_ZN12_GLOBAL__N_12_2E = internal global %"struct.boost::arg.0" zeroinitializer, align 1
@_ZN12_GLOBAL__N_12_3E = internal global %"struct.boost::arg.2" zeroinitializer, align 1
@_ZN12_GLOBAL__N_12_4E = internal global %"struct.boost::arg.4" zeroinitializer, align 1
@_ZN12_GLOBAL__N_12_5E = internal global %"struct.boost::arg.6" zeroinitializer, align 1
@_ZN12_GLOBAL__N_12_6E = internal global %"struct.boost::arg.8" zeroinitializer, align 1
@_ZN12_GLOBAL__N_12_7E = internal global %"struct.boost::arg.10" zeroinitializer, align 1
@_ZN12_GLOBAL__N_12_8E = internal global %"struct.boost::arg.12" zeroinitializer, align 1
@_ZN12_GLOBAL__N_12_9E = internal global %"struct.boost::arg.14" zeroinitializer, align 1
@_ZN5boost6spirit7classicL9nothing_pE = internal global %"struct.boost::spirit::classic::nothing_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL9anychar_pE = internal global %"struct.boost::spirit::classic::anychar_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7alnum_pE = internal global %"struct.boost::spirit::classic::alnum_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7alpha_pE = internal global %"struct.boost::spirit::classic::alpha_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7cntrl_pE = internal global %"struct.boost::spirit::classic::cntrl_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7digit_pE = internal global %"struct.boost::spirit::classic::digit_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7graph_pE = internal global %"struct.boost::spirit::classic::graph_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7lower_pE = internal global %"struct.boost::spirit::classic::lower_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7print_pE = internal global %"struct.boost::spirit::classic::print_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7punct_pE = internal global %"struct.boost::spirit::classic::punct_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7blank_pE = internal global %"struct.boost::spirit::classic::blank_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7space_pE = internal global %"struct.boost::spirit::classic::space_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7upper_pE = internal global %"struct.boost::spirit::classic::upper_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL8xdigit_pE = internal global %"struct.boost::spirit::classic::xdigit_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL5eol_pE = internal global %"struct.boost::spirit::classic::eol_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL5end_pE = internal global %"struct.boost::spirit::classic::end_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL8nocase_dE = internal global %"struct.boost::spirit::classic::inhibit_case_parser_gen" zeroinitializer, align 1
@_ZN5boost6spirit7classicL10as_lower_dE = internal global %"struct.boost::spirit::classic::inhibit_case_parser_gen" zeroinitializer, align 1
@_ZN5boost6spirit7classicL6sign_pE = internal global %"struct.boost::spirit::classic::sign_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL7ureal_pE = internal global %"struct.boost::spirit::classic::real_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL6real_pE = internal global %"struct.boost::spirit::classic::real_parser.45" zeroinitializer, align 1
@_ZN5boost6spirit7classicL14strict_ureal_pE = internal global %"struct.boost::spirit::classic::real_parser.47" zeroinitializer, align 1
@_ZN5boost6spirit7classicL13strict_real_pE = internal global %"struct.boost::spirit::classic::real_parser.49" zeroinitializer, align 1
@_ZN5boost6spirit7classicL9epsilon_pE = internal global %"struct.boost::spirit::classic::epsilon_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL5eps_pE = internal global %"struct.boost::spirit::classic::epsilon_parser" zeroinitializer, align 1
@_ZN5boost6spirit7classicL16refactor_unary_dE = internal global %"class.boost::spirit::classic::refactor_unary_gen" zeroinitializer, align 1
@_ZN5boost6spirit7classicL17refactor_action_dE = internal global %"class.boost::spirit::classic::refactor_action_gen" zeroinitializer, align 1
@_ZN5boost6spirit7classicL15attach_action_dE = internal global %"class.boost::spirit::classic::attach_action_gen" zeroinitializer, align 1
@_ZN5boost6spirit7classicL6list_pE = internal global %"struct.boost::spirit::classic::list_parser_gen" zeroinitializer, align 1
@_ZN7phoenixL4arg1E = internal global %"struct.phoenix::actor" zeroinitializer, align 1
@_ZN7phoenixL4arg2E = internal global %"struct.phoenix::actor.56" zeroinitializer, align 1
@_ZN7phoenixL4arg3E = internal global %"struct.phoenix::actor.58" zeroinitializer, align 1
@_ZN7phoenixL4arg4E = internal global %"struct.phoenix::actor.60" zeroinitializer, align 1
@_ZN7phoenixL4arg5E = internal global %"struct.phoenix::actor.62" zeroinitializer, align 1
@_ZN7phoenixL4arg6E = internal global %"struct.phoenix::actor.64" zeroinitializer, align 1
@_ZTVN5boost13property_tree11ptree_errorE = linkonce_odr unnamed_addr constant [5 x i8*] [i8* null, i8* bitcast ({ i8*, i8*, i8* }* @_ZTIN5boost13property_tree11ptree_errorE to i8*), i8* bitcast (void (%"class.boost::property_tree::ptree_error"*)* @_ZN5boost13property_tree11ptree_errorD2Ev to i8*), i8* bitcast (void (%"class.boost::property_tree::ptree_error"*)* @_ZN5boost13property_tree11ptree_errorD0Ev to i8*), i8* bitcast (i8* (%"class.std::runtime_error"*)* @_ZNKSt13runtime_error4whatEv to i8*)], align 8
@_ZTVN10__cxxabiv120__si_class_type_infoE = external global i8*
@_ZTSN5boost13property_tree11ptree_errorE = linkonce_odr constant [37 x i8] c"N5boost13property_tree11ptree_errorE\00"
@_ZTISt13runtime_error = external constant i8*
@_ZTIN5boost13property_tree11ptree_errorE = linkonce_odr constant { i8*, i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8** @_ZTVN10__cxxabiv120__si_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([37 x i8]* @_ZTSN5boost13property_tree11ptree_errorE, i32 0, i32 0), i8* bitcast (i8** @_ZTISt13runtime_error to i8*) }
@_ZTSN5boost13property_tree14ptree_bad_dataE = linkonce_odr constant [40 x i8] c"N5boost13property_tree14ptree_bad_dataE\00"
@_ZTIN5boost13property_tree14ptree_bad_dataE = linkonce_odr constant { i8*, i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8** @_ZTVN10__cxxabiv120__si_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([40 x i8]* @_ZTSN5boost13property_tree14ptree_bad_dataE, i32 0, i32 0), i8* bitcast ({ i8*, i8*, i8* }* @_ZTIN5boost13property_tree11ptree_errorE to i8*) }
@_ZTSN5boost13property_tree14ptree_bad_pathE = linkonce_odr constant [40 x i8] c"N5boost13property_tree14ptree_bad_pathE\00"
@_ZTIN5boost13property_tree14ptree_bad_pathE = linkonce_odr constant { i8*, i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8** @_ZTVN10__cxxabiv120__si_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([40 x i8]* @_ZTSN5boost13property_tree14ptree_bad_pathE, i32 0, i32 0), i8* bitcast ({ i8*, i8*, i8* }* @_ZTIN5boost13property_tree11ptree_errorE to i8*) }
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_residual.cc, i8* null }]

define internal void @__cxx_global_var_init() section ".text.startup" {
entry:
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* @_ZStL8__ioinit)
  %0 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #2
  ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*) #0

; Function Attrs: nounwind
declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*) #1

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #2

; Function Attrs: nounwind
declare void @_ZNSt13runtime_errorD2Ev(%"class.std::runtime_error"*) #1

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr void @_ZN5boost13property_tree11ptree_errorD0Ev(%"class.boost::property_tree::ptree_error"* %this) unnamed_addr #3 align 2 {
entry:
  %this.addr = alloca %"class.boost::property_tree::ptree_error"*, align 8
  store %"class.boost::property_tree::ptree_error"* %this, %"class.boost::property_tree::ptree_error"** %this.addr, align 8
  %this1 = load %"class.boost::property_tree::ptree_error"** %this.addr
  call void @_ZN5boost13property_tree11ptree_errorD2Ev(%"class.boost::property_tree::ptree_error"* %this1) #2
  %0 = bitcast %"class.boost::property_tree::ptree_error"* %this1 to i8*
  call void @_ZdlPv(i8* %0) #9
  ret void
}

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr void @_ZN5boost13property_tree11ptree_errorD2Ev(%"class.boost::property_tree::ptree_error"* %this) unnamed_addr #3 align 2 {
entry:
  %this.addr = alloca %"class.boost::property_tree::ptree_error"*, align 8
  store %"class.boost::property_tree::ptree_error"* %this, %"class.boost::property_tree::ptree_error"** %this.addr, align 8
  %this1 = load %"class.boost::property_tree::ptree_error"** %this.addr
  %0 = bitcast %"class.boost::property_tree::ptree_error"* %this1 to %"class.std::runtime_error"*
  call void @_ZNSt13runtime_errorD2Ev(%"class.std::runtime_error"* %0) #2
  ret void
}

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8*) #4

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost3anyD2Ev(%"class.boost::any"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"class.boost::any"*, align 8
  store %"class.boost::any"* %this, %"class.boost::any"** %this.addr, align 8
  %this1 = load %"class.boost::any"** %this.addr
  %content = getelementptr inbounds %"class.boost::any"* %this1, i32 0, i32 0
  %0 = load %"class.boost::any::placeholder"** %content, align 8
  %isnull = icmp eq %"class.boost::any::placeholder"* %0, null
  br i1 %isnull, label %delete.end, label %delete.notnull

delete.notnull:                                   ; preds = %entry
  %1 = bitcast %"class.boost::any::placeholder"* %0 to void (%"class.boost::any::placeholder"*)***
  %vtable = load void (%"class.boost::any::placeholder"*)*** %1
  %vfn = getelementptr inbounds void (%"class.boost::any::placeholder"*)** %vtable, i64 1
  %2 = load void (%"class.boost::any::placeholder"*)** %vfn
  call void %2(%"class.boost::any::placeholder"* %0) #2
  br label %delete.end

delete.end:                                       ; preds = %delete.notnull, %entry
  ret void
}

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr void @_ZN5boost13property_tree14ptree_bad_dataD0Ev(%"class.boost::property_tree::ptree_bad_data"* %this) unnamed_addr #3 align 2 {
entry:
  %this.addr = alloca %"class.boost::property_tree::ptree_bad_data"*, align 8
  store %"class.boost::property_tree::ptree_bad_data"* %this, %"class.boost::property_tree::ptree_bad_data"** %this.addr, align 8
  %this1 = load %"class.boost::property_tree::ptree_bad_data"** %this.addr
  call void @_ZN5boost13property_tree14ptree_bad_dataD2Ev(%"class.boost::property_tree::ptree_bad_data"* %this1) #2
  %0 = bitcast %"class.boost::property_tree::ptree_bad_data"* %this1 to i8*
  call void @_ZdlPv(i8* %0) #9
  ret void
}

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr void @_ZN5boost13property_tree14ptree_bad_dataD2Ev(%"class.boost::property_tree::ptree_bad_data"* %this) unnamed_addr #3 align 2 {
entry:
  %this.addr = alloca %"class.boost::property_tree::ptree_bad_data"*, align 8
  store %"class.boost::property_tree::ptree_bad_data"* %this, %"class.boost::property_tree::ptree_bad_data"** %this.addr, align 8
  %this1 = load %"class.boost::property_tree::ptree_bad_data"** %this.addr
  %0 = bitcast %"class.boost::property_tree::ptree_bad_data"* %this1 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([5 x i8*]* @_ZTVN5boost13property_tree14ptree_bad_dataE, i64 0, i64 2) to i32 (...)**), i32 (...)*** %0
  %m_data = getelementptr inbounds %"class.boost::property_tree::ptree_bad_data"* %this1, i32 0, i32 1
  call void @_ZN5boost3anyD2Ev(%"class.boost::any"* %m_data) #2
  %1 = bitcast %"class.boost::property_tree::ptree_bad_data"* %this1 to %"class.boost::property_tree::ptree_error"*
  call void @_ZN5boost13property_tree11ptree_errorD2Ev(%"class.boost::property_tree::ptree_error"* %1) #2
  ret void
}

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr void @_ZN5boost13property_tree14ptree_bad_pathD0Ev(%"class.boost::property_tree::ptree_bad_path"* %this) unnamed_addr #3 align 2 {
entry:
  %this.addr = alloca %"class.boost::property_tree::ptree_bad_path"*, align 8
  store %"class.boost::property_tree::ptree_bad_path"* %this, %"class.boost::property_tree::ptree_bad_path"** %this.addr, align 8
  %this1 = load %"class.boost::property_tree::ptree_bad_path"** %this.addr
  call void @_ZN5boost13property_tree14ptree_bad_pathD2Ev(%"class.boost::property_tree::ptree_bad_path"* %this1) #2
  %0 = bitcast %"class.boost::property_tree::ptree_bad_path"* %this1 to i8*
  call void @_ZdlPv(i8* %0) #9
  ret void
}

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr void @_ZN5boost13property_tree14ptree_bad_pathD2Ev(%"class.boost::property_tree::ptree_bad_path"* %this) unnamed_addr #3 align 2 {
entry:
  %this.addr = alloca %"class.boost::property_tree::ptree_bad_path"*, align 8
  store %"class.boost::property_tree::ptree_bad_path"* %this, %"class.boost::property_tree::ptree_bad_path"** %this.addr, align 8
  %this1 = load %"class.boost::property_tree::ptree_bad_path"** %this.addr
  %0 = bitcast %"class.boost::property_tree::ptree_bad_path"* %this1 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([5 x i8*]* @_ZTVN5boost13property_tree14ptree_bad_pathE, i64 0, i64 2) to i32 (...)**), i32 (...)*** %0
  %m_path = getelementptr inbounds %"class.boost::property_tree::ptree_bad_path"* %this1, i32 0, i32 1
  call void @_ZN5boost3anyD2Ev(%"class.boost::any"* %m_path) #2
  %1 = bitcast %"class.boost::property_tree::ptree_bad_path"* %this1 to %"class.boost::property_tree::ptree_error"*
  call void @_ZN5boost13property_tree11ptree_errorD2Ev(%"class.boost::property_tree::ptree_error"* %1) #2
  ret void
}

define internal void @__cxx_global_var_init1() section ".text.startup" {
entry:
  call void @_ZN5boost3argILi1EEC2Ev(%"struct.boost::arg"* @_ZN12_GLOBAL__N_12_1E)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost3argILi1EEC2Ev(%"struct.boost::arg"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::arg"*, align 8
  store %"struct.boost::arg"* %this, %"struct.boost::arg"** %this.addr, align 8
  %this1 = load %"struct.boost::arg"** %this.addr
  ret void
}

define internal void @__cxx_global_var_init2() section ".text.startup" {
entry:
  call void @_ZN5boost3argILi2EEC2Ev(%"struct.boost::arg.0"* @_ZN12_GLOBAL__N_12_2E)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost3argILi2EEC2Ev(%"struct.boost::arg.0"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::arg.0"*, align 8
  store %"struct.boost::arg.0"* %this, %"struct.boost::arg.0"** %this.addr, align 8
  %this1 = load %"struct.boost::arg.0"** %this.addr
  ret void
}

define internal void @__cxx_global_var_init3() section ".text.startup" {
entry:
  call void @_ZN5boost3argILi3EEC2Ev(%"struct.boost::arg.2"* @_ZN12_GLOBAL__N_12_3E)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost3argILi3EEC2Ev(%"struct.boost::arg.2"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::arg.2"*, align 8
  store %"struct.boost::arg.2"* %this, %"struct.boost::arg.2"** %this.addr, align 8
  %this1 = load %"struct.boost::arg.2"** %this.addr
  ret void
}

define internal void @__cxx_global_var_init4() section ".text.startup" {
entry:
  call void @_ZN5boost3argILi4EEC2Ev(%"struct.boost::arg.4"* @_ZN12_GLOBAL__N_12_4E)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost3argILi4EEC2Ev(%"struct.boost::arg.4"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::arg.4"*, align 8
  store %"struct.boost::arg.4"* %this, %"struct.boost::arg.4"** %this.addr, align 8
  %this1 = load %"struct.boost::arg.4"** %this.addr
  ret void
}

define internal void @__cxx_global_var_init5() section ".text.startup" {
entry:
  call void @_ZN5boost3argILi5EEC2Ev(%"struct.boost::arg.6"* @_ZN12_GLOBAL__N_12_5E)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost3argILi5EEC2Ev(%"struct.boost::arg.6"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::arg.6"*, align 8
  store %"struct.boost::arg.6"* %this, %"struct.boost::arg.6"** %this.addr, align 8
  %this1 = load %"struct.boost::arg.6"** %this.addr
  ret void
}

define internal void @__cxx_global_var_init6() section ".text.startup" {
entry:
  call void @_ZN5boost3argILi6EEC2Ev(%"struct.boost::arg.8"* @_ZN12_GLOBAL__N_12_6E)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost3argILi6EEC2Ev(%"struct.boost::arg.8"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::arg.8"*, align 8
  store %"struct.boost::arg.8"* %this, %"struct.boost::arg.8"** %this.addr, align 8
  %this1 = load %"struct.boost::arg.8"** %this.addr
  ret void
}

define internal void @__cxx_global_var_init7() section ".text.startup" {
entry:
  call void @_ZN5boost3argILi7EEC2Ev(%"struct.boost::arg.10"* @_ZN12_GLOBAL__N_12_7E)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost3argILi7EEC2Ev(%"struct.boost::arg.10"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::arg.10"*, align 8
  store %"struct.boost::arg.10"* %this, %"struct.boost::arg.10"** %this.addr, align 8
  %this1 = load %"struct.boost::arg.10"** %this.addr
  ret void
}

define internal void @__cxx_global_var_init8() section ".text.startup" {
entry:
  call void @_ZN5boost3argILi8EEC2Ev(%"struct.boost::arg.12"* @_ZN12_GLOBAL__N_12_8E)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost3argILi8EEC2Ev(%"struct.boost::arg.12"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::arg.12"*, align 8
  store %"struct.boost::arg.12"* %this, %"struct.boost::arg.12"** %this.addr, align 8
  %this1 = load %"struct.boost::arg.12"** %this.addr
  ret void
}

define internal void @__cxx_global_var_init9() section ".text.startup" {
entry:
  call void @_ZN5boost3argILi9EEC2Ev(%"struct.boost::arg.14"* @_ZN12_GLOBAL__N_12_9E)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost3argILi9EEC2Ev(%"struct.boost::arg.14"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::arg.14"*, align 8
  store %"struct.boost::arg.14"* %this, %"struct.boost::arg.14"** %this.addr, align 8
  %this1 = load %"struct.boost::arg.14"** %this.addr
  ret void
}

define internal void @__cxx_global_var_init10() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic14nothing_parserC2Ev(%"struct.boost::spirit::classic::nothing_parser"* @_ZN5boost6spirit7classicL9nothing_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic14nothing_parserC2Ev(%"struct.boost::spirit::classic::nothing_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::nothing_parser"*, align 8
  store %"struct.boost::spirit::classic::nothing_parser"* %this, %"struct.boost::spirit::classic::nothing_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::nothing_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::nothing_parser"* %this1 to %"struct.boost::spirit::classic::parser"*
  ret void
}

define internal void @__cxx_global_var_init11() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic14anychar_parserC2Ev(%"struct.boost::spirit::classic::anychar_parser"* @_ZN5boost6spirit7classicL9anychar_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic14anychar_parserC2Ev(%"struct.boost::spirit::classic::anychar_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::anychar_parser"*, align 8
  store %"struct.boost::spirit::classic::anychar_parser"* %this, %"struct.boost::spirit::classic::anychar_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::anychar_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::anychar_parser"* %this1 to %"struct.boost::spirit::classic::char_parser"*
  ret void
}

define internal void @__cxx_global_var_init12() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic12alnum_parserC2Ev(%"struct.boost::spirit::classic::alnum_parser"* @_ZN5boost6spirit7classicL7alnum_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic12alnum_parserC2Ev(%"struct.boost::spirit::classic::alnum_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::alnum_parser"*, align 8
  store %"struct.boost::spirit::classic::alnum_parser"* %this, %"struct.boost::spirit::classic::alnum_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::alnum_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::alnum_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.17"*
  ret void
}

define internal void @__cxx_global_var_init13() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic12alpha_parserC2Ev(%"struct.boost::spirit::classic::alpha_parser"* @_ZN5boost6spirit7classicL7alpha_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic12alpha_parserC2Ev(%"struct.boost::spirit::classic::alpha_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::alpha_parser"*, align 8
  store %"struct.boost::spirit::classic::alpha_parser"* %this, %"struct.boost::spirit::classic::alpha_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::alpha_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::alpha_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.19"*
  ret void
}

define internal void @__cxx_global_var_init14() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic12cntrl_parserC2Ev(%"struct.boost::spirit::classic::cntrl_parser"* @_ZN5boost6spirit7classicL7cntrl_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic12cntrl_parserC2Ev(%"struct.boost::spirit::classic::cntrl_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::cntrl_parser"*, align 8
  store %"struct.boost::spirit::classic::cntrl_parser"* %this, %"struct.boost::spirit::classic::cntrl_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::cntrl_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::cntrl_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.21"*
  ret void
}

define internal void @__cxx_global_var_init15() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic12digit_parserC2Ev(%"struct.boost::spirit::classic::digit_parser"* @_ZN5boost6spirit7classicL7digit_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic12digit_parserC2Ev(%"struct.boost::spirit::classic::digit_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::digit_parser"*, align 8
  store %"struct.boost::spirit::classic::digit_parser"* %this, %"struct.boost::spirit::classic::digit_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::digit_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::digit_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.23"*
  ret void
}

define internal void @__cxx_global_var_init16() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic12graph_parserC2Ev(%"struct.boost::spirit::classic::graph_parser"* @_ZN5boost6spirit7classicL7graph_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic12graph_parserC2Ev(%"struct.boost::spirit::classic::graph_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::graph_parser"*, align 8
  store %"struct.boost::spirit::classic::graph_parser"* %this, %"struct.boost::spirit::classic::graph_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::graph_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::graph_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.25"*
  ret void
}

define internal void @__cxx_global_var_init17() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic12lower_parserC2Ev(%"struct.boost::spirit::classic::lower_parser"* @_ZN5boost6spirit7classicL7lower_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic12lower_parserC2Ev(%"struct.boost::spirit::classic::lower_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::lower_parser"*, align 8
  store %"struct.boost::spirit::classic::lower_parser"* %this, %"struct.boost::spirit::classic::lower_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::lower_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::lower_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.27"*
  ret void
}

define internal void @__cxx_global_var_init18() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic12print_parserC2Ev(%"struct.boost::spirit::classic::print_parser"* @_ZN5boost6spirit7classicL7print_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic12print_parserC2Ev(%"struct.boost::spirit::classic::print_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::print_parser"*, align 8
  store %"struct.boost::spirit::classic::print_parser"* %this, %"struct.boost::spirit::classic::print_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::print_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::print_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.29"*
  ret void
}

define internal void @__cxx_global_var_init19() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic12punct_parserC2Ev(%"struct.boost::spirit::classic::punct_parser"* @_ZN5boost6spirit7classicL7punct_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic12punct_parserC2Ev(%"struct.boost::spirit::classic::punct_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::punct_parser"*, align 8
  store %"struct.boost::spirit::classic::punct_parser"* %this, %"struct.boost::spirit::classic::punct_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::punct_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::punct_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.31"*
  ret void
}

define internal void @__cxx_global_var_init20() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic12blank_parserC2Ev(%"struct.boost::spirit::classic::blank_parser"* @_ZN5boost6spirit7classicL7blank_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic12blank_parserC2Ev(%"struct.boost::spirit::classic::blank_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::blank_parser"*, align 8
  store %"struct.boost::spirit::classic::blank_parser"* %this, %"struct.boost::spirit::classic::blank_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::blank_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::blank_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.33"*
  ret void
}

define internal void @__cxx_global_var_init21() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic12space_parserC2Ev(%"struct.boost::spirit::classic::space_parser"* @_ZN5boost6spirit7classicL7space_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic12space_parserC2Ev(%"struct.boost::spirit::classic::space_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::space_parser"*, align 8
  store %"struct.boost::spirit::classic::space_parser"* %this, %"struct.boost::spirit::classic::space_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::space_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::space_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.35"*
  ret void
}

define internal void @__cxx_global_var_init22() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic12upper_parserC2Ev(%"struct.boost::spirit::classic::upper_parser"* @_ZN5boost6spirit7classicL7upper_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic12upper_parserC2Ev(%"struct.boost::spirit::classic::upper_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::upper_parser"*, align 8
  store %"struct.boost::spirit::classic::upper_parser"* %this, %"struct.boost::spirit::classic::upper_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::upper_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::upper_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.37"*
  ret void
}

define internal void @__cxx_global_var_init23() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic13xdigit_parserC2Ev(%"struct.boost::spirit::classic::xdigit_parser"* @_ZN5boost6spirit7classicL8xdigit_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic13xdigit_parserC2Ev(%"struct.boost::spirit::classic::xdigit_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::xdigit_parser"*, align 8
  store %"struct.boost::spirit::classic::xdigit_parser"* %this, %"struct.boost::spirit::classic::xdigit_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::xdigit_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::xdigit_parser"* %this1 to %"struct.boost::spirit::classic::char_parser.39"*
  ret void
}

define internal void @__cxx_global_var_init24() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic10eol_parserC2Ev(%"struct.boost::spirit::classic::eol_parser"* @_ZN5boost6spirit7classicL5eol_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic10eol_parserC2Ev(%"struct.boost::spirit::classic::eol_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::eol_parser"*, align 8
  store %"struct.boost::spirit::classic::eol_parser"* %this, %"struct.boost::spirit::classic::eol_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::eol_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::eol_parser"* %this1 to %"struct.boost::spirit::classic::parser.41"*
  ret void
}

define internal void @__cxx_global_var_init25() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic10end_parserC2Ev(%"struct.boost::spirit::classic::end_parser"* @_ZN5boost6spirit7classicL5end_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic10end_parserC2Ev(%"struct.boost::spirit::classic::end_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::end_parser"*, align 8
  store %"struct.boost::spirit::classic::end_parser"* %this, %"struct.boost::spirit::classic::end_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::end_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::end_parser"* %this1 to %"struct.boost::spirit::classic::parser.42"*
  ret void
}

define internal void @__cxx_global_var_init26() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic23inhibit_case_parser_genC2Ev(%"struct.boost::spirit::classic::inhibit_case_parser_gen"* @_ZN5boost6spirit7classicL8nocase_dE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic23inhibit_case_parser_genC2Ev(%"struct.boost::spirit::classic::inhibit_case_parser_gen"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::inhibit_case_parser_gen"*, align 8
  store %"struct.boost::spirit::classic::inhibit_case_parser_gen"* %this, %"struct.boost::spirit::classic::inhibit_case_parser_gen"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::inhibit_case_parser_gen"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::inhibit_case_parser_gen"* %this1 to %"struct.boost::spirit::classic::inhibit_case_parser_gen_base"*
  ret void
}

define internal void @__cxx_global_var_init27() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic23inhibit_case_parser_genC2Ev(%"struct.boost::spirit::classic::inhibit_case_parser_gen"* @_ZN5boost6spirit7classicL10as_lower_dE)
  ret void
}

define internal void @__cxx_global_var_init28() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic11sign_parserC2Ev(%"struct.boost::spirit::classic::sign_parser"* @_ZN5boost6spirit7classicL6sign_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic11sign_parserC2Ev(%"struct.boost::spirit::classic::sign_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::sign_parser"*, align 8
  store %"struct.boost::spirit::classic::sign_parser"* %this, %"struct.boost::spirit::classic::sign_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::sign_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::sign_parser"* %this1 to %"struct.boost::spirit::classic::parser.43"*
  ret void
}

define internal void @__cxx_global_var_init29() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic11real_parserIdNS1_21ureal_parser_policiesIdEEEC2Ev(%"struct.boost::spirit::classic::real_parser"* @_ZN5boost6spirit7classicL7ureal_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic11real_parserIdNS1_21ureal_parser_policiesIdEEEC2Ev(%"struct.boost::spirit::classic::real_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::real_parser"*, align 8
  store %"struct.boost::spirit::classic::real_parser"* %this, %"struct.boost::spirit::classic::real_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::real_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::real_parser"* %this1 to %"struct.boost::spirit::classic::parser.44"*
  ret void
}

define internal void @__cxx_global_var_init30() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic11real_parserIdNS1_20real_parser_policiesIdEEEC2Ev(%"struct.boost::spirit::classic::real_parser.45"* @_ZN5boost6spirit7classicL6real_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic11real_parserIdNS1_20real_parser_policiesIdEEEC2Ev(%"struct.boost::spirit::classic::real_parser.45"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::real_parser.45"*, align 8
  store %"struct.boost::spirit::classic::real_parser.45"* %this, %"struct.boost::spirit::classic::real_parser.45"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::real_parser.45"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::real_parser.45"* %this1 to %"struct.boost::spirit::classic::parser.46"*
  ret void
}

define internal void @__cxx_global_var_init31() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic11real_parserIdNS1_28strict_ureal_parser_policiesIdEEEC2Ev(%"struct.boost::spirit::classic::real_parser.47"* @_ZN5boost6spirit7classicL14strict_ureal_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic11real_parserIdNS1_28strict_ureal_parser_policiesIdEEEC2Ev(%"struct.boost::spirit::classic::real_parser.47"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::real_parser.47"*, align 8
  store %"struct.boost::spirit::classic::real_parser.47"* %this, %"struct.boost::spirit::classic::real_parser.47"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::real_parser.47"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::real_parser.47"* %this1 to %"struct.boost::spirit::classic::parser.48"*
  ret void
}

define internal void @__cxx_global_var_init32() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic11real_parserIdNS1_27strict_real_parser_policiesIdEEEC2Ev(%"struct.boost::spirit::classic::real_parser.49"* @_ZN5boost6spirit7classicL13strict_real_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic11real_parserIdNS1_27strict_real_parser_policiesIdEEEC2Ev(%"struct.boost::spirit::classic::real_parser.49"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::real_parser.49"*, align 8
  store %"struct.boost::spirit::classic::real_parser.49"* %this, %"struct.boost::spirit::classic::real_parser.49"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::real_parser.49"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::real_parser.49"* %this1 to %"struct.boost::spirit::classic::parser.50"*
  ret void
}

define internal void @__cxx_global_var_init33() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic14epsilon_parserC2Ev(%"struct.boost::spirit::classic::epsilon_parser"* @_ZN5boost6spirit7classicL9epsilon_pE)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic14epsilon_parserC2Ev(%"struct.boost::spirit::classic::epsilon_parser"* %this) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::epsilon_parser"*, align 8
  store %"struct.boost::spirit::classic::epsilon_parser"* %this, %"struct.boost::spirit::classic::epsilon_parser"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::epsilon_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::epsilon_parser"* %this1 to %"struct.boost::spirit::classic::parser.51"*
  ret void
}

define internal void @__cxx_global_var_init34() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic14epsilon_parserC2Ev(%"struct.boost::spirit::classic::epsilon_parser"* @_ZN5boost6spirit7classicL5eps_pE)
  ret void
}

define internal void @__cxx_global_var_init35() section ".text.startup" {
entry:
  %ref.tmp = alloca %"struct.boost::spirit::classic::non_nested_refactoring", align 1
  call void @_ZN5boost6spirit7classic18refactor_unary_genINS1_22non_nested_refactoringEEC2ERKS3_(%"class.boost::spirit::classic::refactor_unary_gen"* @_ZN5boost6spirit7classicL16refactor_unary_dE, %"struct.boost::spirit::classic::non_nested_refactoring"* dereferenceable(1) %ref.tmp)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic18refactor_unary_genINS1_22non_nested_refactoringEEC2ERKS3_(%"class.boost::spirit::classic::refactor_unary_gen"* %this, %"struct.boost::spirit::classic::non_nested_refactoring"* dereferenceable(1) %nested_) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"class.boost::spirit::classic::refactor_unary_gen"*, align 8
  %nested_.addr = alloca %"struct.boost::spirit::classic::non_nested_refactoring"*, align 8
  store %"class.boost::spirit::classic::refactor_unary_gen"* %this, %"class.boost::spirit::classic::refactor_unary_gen"** %this.addr, align 8
  store %"struct.boost::spirit::classic::non_nested_refactoring"* %nested_, %"struct.boost::spirit::classic::non_nested_refactoring"** %nested_.addr, align 8
  %this1 = load %"class.boost::spirit::classic::refactor_unary_gen"** %this.addr
  %nested = getelementptr inbounds %"class.boost::spirit::classic::refactor_unary_gen"* %this1, i32 0, i32 0
  %0 = load %"struct.boost::spirit::classic::non_nested_refactoring"** %nested_.addr, align 8
  ret void
}

define internal void @__cxx_global_var_init36() section ".text.startup" {
entry:
  %ref.tmp = alloca %"struct.boost::spirit::classic::non_nested_refactoring", align 1
  call void @_ZN5boost6spirit7classic19refactor_action_genINS1_22non_nested_refactoringEEC2ERKS3_(%"class.boost::spirit::classic::refactor_action_gen"* @_ZN5boost6spirit7classicL17refactor_action_dE, %"struct.boost::spirit::classic::non_nested_refactoring"* dereferenceable(1) %ref.tmp)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic19refactor_action_genINS1_22non_nested_refactoringEEC2ERKS3_(%"class.boost::spirit::classic::refactor_action_gen"* %this, %"struct.boost::spirit::classic::non_nested_refactoring"* dereferenceable(1) %nested_) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"class.boost::spirit::classic::refactor_action_gen"*, align 8
  %nested_.addr = alloca %"struct.boost::spirit::classic::non_nested_refactoring"*, align 8
  store %"class.boost::spirit::classic::refactor_action_gen"* %this, %"class.boost::spirit::classic::refactor_action_gen"** %this.addr, align 8
  store %"struct.boost::spirit::classic::non_nested_refactoring"* %nested_, %"struct.boost::spirit::classic::non_nested_refactoring"** %nested_.addr, align 8
  %this1 = load %"class.boost::spirit::classic::refactor_action_gen"** %this.addr
  %nested = getelementptr inbounds %"class.boost::spirit::classic::refactor_action_gen"* %this1, i32 0, i32 0
  %0 = load %"struct.boost::spirit::classic::non_nested_refactoring"** %nested_.addr, align 8
  ret void
}

define internal void @__cxx_global_var_init37() section ".text.startup" {
entry:
  %ref.tmp = alloca %"struct.boost::spirit::classic::non_nested_refactoring", align 1
  call void @_ZN5boost6spirit7classic17attach_action_genINS1_22non_nested_refactoringEEC2ERKS3_(%"class.boost::spirit::classic::attach_action_gen"* @_ZN5boost6spirit7classicL15attach_action_dE, %"struct.boost::spirit::classic::non_nested_refactoring"* dereferenceable(1) %ref.tmp)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic17attach_action_genINS1_22non_nested_refactoringEEC2ERKS3_(%"class.boost::spirit::classic::attach_action_gen"* %this, %"struct.boost::spirit::classic::non_nested_refactoring"* dereferenceable(1) %nested_) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"class.boost::spirit::classic::attach_action_gen"*, align 8
  %nested_.addr = alloca %"struct.boost::spirit::classic::non_nested_refactoring"*, align 8
  store %"class.boost::spirit::classic::attach_action_gen"* %this, %"class.boost::spirit::classic::attach_action_gen"** %this.addr, align 8
  store %"struct.boost::spirit::classic::non_nested_refactoring"* %nested_, %"struct.boost::spirit::classic::non_nested_refactoring"** %nested_.addr, align 8
  %this1 = load %"class.boost::spirit::classic::attach_action_gen"** %this.addr
  %nested = getelementptr inbounds %"class.boost::spirit::classic::attach_action_gen"* %this1, i32 0, i32 0
  %0 = load %"struct.boost::spirit::classic::non_nested_refactoring"** %nested_.addr, align 8
  ret void
}

define internal void @__cxx_global_var_init38() section ".text.startup" {
entry:
  call void @_ZN5boost6spirit7classic15list_parser_genIcEC2Ev(%"struct.boost::spirit::classic::list_parser_gen"* @_ZN5boost6spirit7classicL6list_pE)
  ret void
}

; Function Attrs: uwtable
define linkonce_odr void @_ZN5boost6spirit7classic15list_parser_genIcEC2Ev(%"struct.boost::spirit::classic::list_parser_gen"* %this) unnamed_addr #6 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::list_parser_gen"*, align 8
  %ref.tmp = alloca %"struct.boost::spirit::classic::kleene_star", align 1
  %undef.agg.tmp = alloca %"struct.boost::spirit::classic::kleene_star", align 1
  %ref.tmp2 = alloca %"struct.boost::spirit::classic::chlit", align 1
  %ref.tmp3 = alloca %"struct.boost::spirit::classic::no_list_endtoken", align 1
  store %"struct.boost::spirit::classic::list_parser_gen"* %this, %"struct.boost::spirit::classic::list_parser_gen"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::list_parser_gen"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::list_parser_gen"* %this1 to %"struct.boost::spirit::classic::list_parser"*
  call void @_ZN5boost6spirit7classicdeINS1_14anychar_parserEEENS1_11kleene_starIT_EERKNS1_6parserIS5_EE(%"struct.boost::spirit::classic::parser.16"* dereferenceable(1) bitcast (%"struct.boost::spirit::classic::anychar_parser"* @_ZN5boost6spirit7classicL9anychar_pE to %"struct.boost::spirit::classic::parser.16"*))
  call void @_ZN5boost6spirit7classic5chlitIcEC2Ec(%"struct.boost::spirit::classic::chlit"* %ref.tmp2, i8 signext 44)
  call void @_ZN5boost6spirit7classic11list_parserINS1_11kleene_starINS1_14anychar_parserEEENS1_5chlitIcEENS1_16no_list_endtokenENS1_21plain_parser_categoryEEC2ERKS5_RKS7_RKS8_(%"struct.boost::spirit::classic::list_parser"* %0, %"struct.boost::spirit::classic::kleene_star"* dereferenceable(1) %ref.tmp, %"struct.boost::spirit::classic::chlit"* dereferenceable(1) %ref.tmp2, %"struct.boost::spirit::classic::no_list_endtoken"* dereferenceable(1) %ref.tmp3)
  ret void
}

define internal void @__cxx_global_var_init39() section ".text.startup" {
entry:
  %ref.tmp = alloca %"struct.phoenix::argument", align 1
  call void @_ZN7phoenix5actorINS_8argumentILi0EEEEC2ERKS2_(%"struct.phoenix::actor"* @_ZN7phoenixL4arg1E, %"struct.phoenix::argument"* dereferenceable(1) %ref.tmp)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN7phoenix5actorINS_8argumentILi0EEEEC2ERKS2_(%"struct.phoenix::actor"* %this, %"struct.phoenix::argument"* dereferenceable(1) %base) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.phoenix::actor"*, align 8
  %base.addr = alloca %"struct.phoenix::argument"*, align 8
  store %"struct.phoenix::actor"* %this, %"struct.phoenix::actor"** %this.addr, align 8
  store %"struct.phoenix::argument"* %base, %"struct.phoenix::argument"** %base.addr, align 8
  %this1 = load %"struct.phoenix::actor"** %this.addr
  %0 = bitcast %"struct.phoenix::actor"* %this1 to %"struct.phoenix::argument"*
  %1 = load %"struct.phoenix::argument"** %base.addr, align 8
  ret void
}

define internal void @__cxx_global_var_init40() section ".text.startup" {
entry:
  %ref.tmp = alloca %"struct.phoenix::argument.57", align 1
  call void @_ZN7phoenix5actorINS_8argumentILi1EEEEC2ERKS2_(%"struct.phoenix::actor.56"* @_ZN7phoenixL4arg2E, %"struct.phoenix::argument.57"* dereferenceable(1) %ref.tmp)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN7phoenix5actorINS_8argumentILi1EEEEC2ERKS2_(%"struct.phoenix::actor.56"* %this, %"struct.phoenix::argument.57"* dereferenceable(1) %base) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.phoenix::actor.56"*, align 8
  %base.addr = alloca %"struct.phoenix::argument.57"*, align 8
  store %"struct.phoenix::actor.56"* %this, %"struct.phoenix::actor.56"** %this.addr, align 8
  store %"struct.phoenix::argument.57"* %base, %"struct.phoenix::argument.57"** %base.addr, align 8
  %this1 = load %"struct.phoenix::actor.56"** %this.addr
  %0 = bitcast %"struct.phoenix::actor.56"* %this1 to %"struct.phoenix::argument.57"*
  %1 = load %"struct.phoenix::argument.57"** %base.addr, align 8
  ret void
}

define internal void @__cxx_global_var_init41() section ".text.startup" {
entry:
  %ref.tmp = alloca %"struct.phoenix::argument.59", align 1
  call void @_ZN7phoenix5actorINS_8argumentILi2EEEEC2ERKS2_(%"struct.phoenix::actor.58"* @_ZN7phoenixL4arg3E, %"struct.phoenix::argument.59"* dereferenceable(1) %ref.tmp)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN7phoenix5actorINS_8argumentILi2EEEEC2ERKS2_(%"struct.phoenix::actor.58"* %this, %"struct.phoenix::argument.59"* dereferenceable(1) %base) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.phoenix::actor.58"*, align 8
  %base.addr = alloca %"struct.phoenix::argument.59"*, align 8
  store %"struct.phoenix::actor.58"* %this, %"struct.phoenix::actor.58"** %this.addr, align 8
  store %"struct.phoenix::argument.59"* %base, %"struct.phoenix::argument.59"** %base.addr, align 8
  %this1 = load %"struct.phoenix::actor.58"** %this.addr
  %0 = bitcast %"struct.phoenix::actor.58"* %this1 to %"struct.phoenix::argument.59"*
  %1 = load %"struct.phoenix::argument.59"** %base.addr, align 8
  ret void
}

define internal void @__cxx_global_var_init42() section ".text.startup" {
entry:
  %ref.tmp = alloca %"struct.phoenix::argument.61", align 1
  call void @_ZN7phoenix5actorINS_8argumentILi3EEEEC2ERKS2_(%"struct.phoenix::actor.60"* @_ZN7phoenixL4arg4E, %"struct.phoenix::argument.61"* dereferenceable(1) %ref.tmp)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN7phoenix5actorINS_8argumentILi3EEEEC2ERKS2_(%"struct.phoenix::actor.60"* %this, %"struct.phoenix::argument.61"* dereferenceable(1) %base) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.phoenix::actor.60"*, align 8
  %base.addr = alloca %"struct.phoenix::argument.61"*, align 8
  store %"struct.phoenix::actor.60"* %this, %"struct.phoenix::actor.60"** %this.addr, align 8
  store %"struct.phoenix::argument.61"* %base, %"struct.phoenix::argument.61"** %base.addr, align 8
  %this1 = load %"struct.phoenix::actor.60"** %this.addr
  %0 = bitcast %"struct.phoenix::actor.60"* %this1 to %"struct.phoenix::argument.61"*
  %1 = load %"struct.phoenix::argument.61"** %base.addr, align 8
  ret void
}

define internal void @__cxx_global_var_init43() section ".text.startup" {
entry:
  %ref.tmp = alloca %"struct.phoenix::argument.63", align 1
  call void @_ZN7phoenix5actorINS_8argumentILi4EEEEC2ERKS2_(%"struct.phoenix::actor.62"* @_ZN7phoenixL4arg5E, %"struct.phoenix::argument.63"* dereferenceable(1) %ref.tmp)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN7phoenix5actorINS_8argumentILi4EEEEC2ERKS2_(%"struct.phoenix::actor.62"* %this, %"struct.phoenix::argument.63"* dereferenceable(1) %base) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.phoenix::actor.62"*, align 8
  %base.addr = alloca %"struct.phoenix::argument.63"*, align 8
  store %"struct.phoenix::actor.62"* %this, %"struct.phoenix::actor.62"** %this.addr, align 8
  store %"struct.phoenix::argument.63"* %base, %"struct.phoenix::argument.63"** %base.addr, align 8
  %this1 = load %"struct.phoenix::actor.62"** %this.addr
  %0 = bitcast %"struct.phoenix::actor.62"* %this1 to %"struct.phoenix::argument.63"*
  %1 = load %"struct.phoenix::argument.63"** %base.addr, align 8
  ret void
}

define internal void @__cxx_global_var_init44() section ".text.startup" {
entry:
  %ref.tmp = alloca %"struct.phoenix::argument.65", align 1
  call void @_ZN7phoenix5actorINS_8argumentILi5EEEEC2ERKS2_(%"struct.phoenix::actor.64"* @_ZN7phoenixL4arg6E, %"struct.phoenix::argument.65"* dereferenceable(1) %ref.tmp)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN7phoenix5actorINS_8argumentILi5EEEEC2ERKS2_(%"struct.phoenix::actor.64"* %this, %"struct.phoenix::argument.65"* dereferenceable(1) %base) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.phoenix::actor.64"*, align 8
  %base.addr = alloca %"struct.phoenix::argument.65"*, align 8
  store %"struct.phoenix::actor.64"* %this, %"struct.phoenix::actor.64"** %this.addr, align 8
  store %"struct.phoenix::argument.65"* %base, %"struct.phoenix::argument.65"** %base.addr, align 8
  %this1 = load %"struct.phoenix::actor.64"** %this.addr
  %0 = bitcast %"struct.phoenix::actor.64"* %this1 to %"struct.phoenix::argument.65"*
  %1 = load %"struct.phoenix::argument.65"** %base.addr, align 8
  ret void
}

; Function Attrs: uwtable
define void @_ZN5boxmg5bmg2d6kernel5impls8residualERKNS0_4core9StencilOpERKNS3_5ArrayIjdEESA_RS8_(%"class.boxmg::bmg2d::core::StencilOp"* dereferenceable(120) %A, %"class.boxmg::bmg2d::core::Array.72"* dereferenceable(88) %x, %"class.boxmg::bmg2d::core::Array.72"* dereferenceable(88) %b, %"class.boxmg::bmg2d::core::Array.72"* dereferenceable(88) %r) #6 {
entry:
  %A.addr = alloca %"class.boxmg::bmg2d::core::StencilOp"*, align 8
  %x.addr = alloca %"class.boxmg::bmg2d::core::Array.72"*, align 8
  %b.addr = alloca %"class.boxmg::bmg2d::core::Array.72"*, align 8
  %r.addr = alloca %"class.boxmg::bmg2d::core::Array.72"*, align 8
  %so = alloca %"class.boxmg::bmg2d::core::GridStencil"*, align 8
  %__range = alloca %"class.boost::iterator_range"*, align 8
  %__begin = alloca %"class.boost::counting_iterator", align 4
  %__end = alloca %"class.boost::counting_iterator", align 4
  %j = alloca i32, align 4
  %__range4 = alloca %"class.boost::iterator_range"*, align 8
  %__begin8 = alloca %"class.boost::counting_iterator", align 4
  %__end9 = alloca %"class.boost::counting_iterator", align 4
  %i = alloca i32, align 4
  store %"class.boxmg::bmg2d::core::StencilOp"* %A, %"class.boxmg::bmg2d::core::StencilOp"** %A.addr, align 8
  store %"class.boxmg::bmg2d::core::Array.72"* %x, %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  store %"class.boxmg::bmg2d::core::Array.72"* %b, %"class.boxmg::bmg2d::core::Array.72"** %b.addr, align 8
  store %"class.boxmg::bmg2d::core::Array.72"* %r, %"class.boxmg::bmg2d::core::Array.72"** %r.addr, align 8
  %0 = load %"class.boxmg::bmg2d::core::StencilOp"** %A.addr, align 8
  %call = call dereferenceable(112) %"class.boxmg::bmg2d::core::GridStencil"* @_ZNK5boxmg5bmg2d4core9StencilOp7stencilEv(%"class.boxmg::bmg2d::core::StencilOp"* %0)
  store %"class.boxmg::bmg2d::core::GridStencil"* %call, %"class.boxmg::bmg2d::core::GridStencil"** %so, align 8
  %1 = load %"class.boxmg::bmg2d::core::Array.72"** %r.addr, align 8
  %2 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %1 to %"class.boost::iterator_range"* (%"class.boxmg::bmg2d::core::Array.72"*, i32)***
  %vtable = load %"class.boost::iterator_range"* (%"class.boxmg::bmg2d::core::Array.72"*, i32)*** %2
  %vfn = getelementptr inbounds %"class.boost::iterator_range"* (%"class.boxmg::bmg2d::core::Array.72"*, i32)** %vtable, i64 5
  %3 = load %"class.boost::iterator_range"* (%"class.boxmg::bmg2d::core::Array.72"*, i32)** %vfn
  %call1 = call dereferenceable(8) %"class.boost::iterator_range"* %3(%"class.boxmg::bmg2d::core::Array.72"* %1, i32 1)
  store %"class.boost::iterator_range"* %call1, %"class.boost::iterator_range"** %__range, align 8
  %4 = load %"class.boost::iterator_range"** %__range, align 8
  call void @_ZNK5boost14iterator_rangeINS_17counting_iteratorIjNS_11use_defaultES2_EEE5beginEv(%"class.boost::counting_iterator"* sret %__begin, %"class.boost::iterator_range"* %4)
  %5 = load %"class.boost::iterator_range"** %__range, align 8
  call void @_ZNK5boost14iterator_rangeINS_17counting_iteratorIjNS_11use_defaultES2_EEE3endEv(%"class.boost::counting_iterator"* sret %__end, %"class.boost::iterator_range"* %5)
  br label %for.cond

for.cond:                                         ; preds = %for.inc84, %entry
  %6 = bitcast %"class.boost::counting_iterator"* %__begin to %"class.boost::iterator_facade"*
  %7 = bitcast %"class.boost::counting_iterator"* %__end to %"class.boost::iterator_facade"*
  %call2 = call zeroext i1 @_ZN5boostneINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlS3_jS4_S6_lEENS_6detail23enable_if_interoperableIT_T4_NS_3mpl6apply2INS7_12always_bool2ES9_SA_E4typeEE4typeERKNS_15iterator_facadeIS9_T0_T1_T2_T3_EERKNSI_ISA_T5_T6_T7_T8_EE(%"class.boost::iterator_facade"* dereferenceable(1) %6, %"class.boost::iterator_facade"* dereferenceable(1) %7)
  br i1 %call2, label %for.body, label %for.end86

for.body:                                         ; preds = %for.cond
  %8 = bitcast %"class.boost::counting_iterator"* %__begin to %"class.boost::iterator_facade"*
  %call3 = call dereferenceable(4) i32* @_ZNK5boost15iterator_facadeINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlEdeEv(%"class.boost::iterator_facade"* %8)
  %9 = load i32* %call3
  store i32 %9, i32* %j, align 4
  %10 = load %"class.boxmg::bmg2d::core::Array.72"** %r.addr, align 8
  %11 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %10 to %"class.boost::iterator_range"* (%"class.boxmg::bmg2d::core::Array.72"*, i32)***
  %vtable5 = load %"class.boost::iterator_range"* (%"class.boxmg::bmg2d::core::Array.72"*, i32)*** %11
  %vfn6 = getelementptr inbounds %"class.boost::iterator_range"* (%"class.boxmg::bmg2d::core::Array.72"*, i32)** %vtable5, i64 5
  %12 = load %"class.boost::iterator_range"* (%"class.boxmg::bmg2d::core::Array.72"*, i32)** %vfn6
  %call7 = call dereferenceable(8) %"class.boost::iterator_range"* %12(%"class.boxmg::bmg2d::core::Array.72"* %10, i32 0)
  store %"class.boost::iterator_range"* %call7, %"class.boost::iterator_range"** %__range4, align 8
  %13 = load %"class.boost::iterator_range"** %__range4, align 8
  call void @_ZNK5boost14iterator_rangeINS_17counting_iteratorIjNS_11use_defaultES2_EEE5beginEv(%"class.boost::counting_iterator"* sret %__begin8, %"class.boost::iterator_range"* %13)
  %14 = load %"class.boost::iterator_range"** %__range4, align 8
  call void @_ZNK5boost14iterator_rangeINS_17counting_iteratorIjNS_11use_defaultES2_EEE3endEv(%"class.boost::counting_iterator"* sret %__end9, %"class.boost::iterator_range"* %14)
  br label %for.cond10

for.cond10:                                       ; preds = %for.inc, %for.body
  %15 = bitcast %"class.boost::counting_iterator"* %__begin8 to %"class.boost::iterator_facade"*
  %16 = bitcast %"class.boost::counting_iterator"* %__end9 to %"class.boost::iterator_facade"*
  %call11 = call zeroext i1 @_ZN5boostneINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlS3_jS4_S6_lEENS_6detail23enable_if_interoperableIT_T4_NS_3mpl6apply2INS7_12always_bool2ES9_SA_E4typeEE4typeERKNS_15iterator_facadeIS9_T0_T1_T2_T3_EERKNSI_ISA_T5_T6_T7_T8_EE(%"class.boost::iterator_facade"* dereferenceable(1) %15, %"class.boost::iterator_facade"* dereferenceable(1) %16)
  br i1 %call11, label %for.body12, label %for.end

for.body12:                                       ; preds = %for.cond10
  %17 = bitcast %"class.boost::counting_iterator"* %__begin8 to %"class.boost::iterator_facade"*
  %call13 = call dereferenceable(4) i32* @_ZNK5boost15iterator_facadeINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlEdeEv(%"class.boost::iterator_facade"* %17)
  %18 = load i32* %call13
  store i32 %18, i32* %i, align 4
  %19 = load %"class.boxmg::bmg2d::core::Array.72"** %b.addr, align 8
  %20 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %19 to double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)***
  %vtable14 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)*** %20
  %vfn15 = getelementptr inbounds double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vtable14, i64 1
  %21 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vfn15
  %22 = load i32* %i, align 4
  %23 = load i32* %j, align 4
  %call16 = call double %21(%"class.boxmg::bmg2d::core::Array.72"* %19, i32 %22, i32 %23)
  %24 = load %"class.boxmg::bmg2d::core::GridStencil"** %so, align 8
  %25 = load i32* %i, align 4
  %26 = load i32* %j, align 4
  %call17 = call double @_ZNK5boxmg5bmg2d4core11GridStencilclEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %24, i32 %25, i32 %26, i32 1)
  %27 = load %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  %28 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %27 to double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)***
  %vtable18 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)*** %28
  %vfn19 = getelementptr inbounds double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vtable18, i64 1
  %29 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vfn19
  %30 = load i32* %i, align 4
  %sub = sub i32 %30, 1
  %31 = load i32* %j, align 4
  %call20 = call double %29(%"class.boxmg::bmg2d::core::Array.72"* %27, i32 %sub, i32 %31)
  %mul = fmul double %call17, %call20
  %32 = load %"class.boxmg::bmg2d::core::GridStencil"** %so, align 8
  %33 = load i32* %i, align 4
  %34 = load i32* %j, align 4
  %call21 = call double @_ZNK5boxmg5bmg2d4core11GridStencilclEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %32, i32 %33, i32 %34, i32 8)
  %35 = load %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  %36 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %35 to double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)***
  %vtable22 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)*** %36
  %vfn23 = getelementptr inbounds double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vtable22, i64 1
  %37 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vfn23
  %38 = load i32* %i, align 4
  %add = add i32 %38, 1
  %39 = load i32* %j, align 4
  %call24 = call double %37(%"class.boxmg::bmg2d::core::Array.72"* %35, i32 %add, i32 %39)
  %mul25 = fmul double %call21, %call24
  %add26 = fadd double %mul, %mul25
  %40 = load %"class.boxmg::bmg2d::core::GridStencil"** %so, align 8
  %41 = load i32* %i, align 4
  %42 = load i32* %j, align 4
  %call27 = call double @_ZNK5boxmg5bmg2d4core11GridStencilclEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %40, i32 %41, i32 %42, i32 2)
  %43 = load %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  %44 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %43 to double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)***
  %vtable28 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)*** %44
  %vfn29 = getelementptr inbounds double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vtable28, i64 1
  %45 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vfn29
  %46 = load i32* %i, align 4
  %47 = load i32* %j, align 4
  %sub30 = sub i32 %47, 1
  %call31 = call double %45(%"class.boxmg::bmg2d::core::Array.72"* %43, i32 %46, i32 %sub30)
  %mul32 = fmul double %call27, %call31
  %add33 = fadd double %add26, %mul32
  %48 = load %"class.boxmg::bmg2d::core::GridStencil"** %so, align 8
  %49 = load i32* %i, align 4
  %50 = load i32* %j, align 4
  %call34 = call double @_ZNK5boxmg5bmg2d4core11GridStencilclEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %48, i32 %49, i32 %50, i32 6)
  %51 = load %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  %52 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %51 to double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)***
  %vtable35 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)*** %52
  %vfn36 = getelementptr inbounds double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vtable35, i64 1
  %53 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vfn36
  %54 = load i32* %i, align 4
  %55 = load i32* %j, align 4
  %add37 = add i32 %55, 1
  %call38 = call double %53(%"class.boxmg::bmg2d::core::Array.72"* %51, i32 %54, i32 %add37)
  %mul39 = fmul double %call34, %call38
  %add40 = fadd double %add33, %mul39
  %56 = load %"class.boxmg::bmg2d::core::GridStencil"** %so, align 8
  %57 = load i32* %i, align 4
  %58 = load i32* %j, align 4
  %call41 = call double @_ZNK5boxmg5bmg2d4core11GridStencilclEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %56, i32 %57, i32 %58, i32 3)
  %59 = load %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  %60 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %59 to double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)***
  %vtable42 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)*** %60
  %vfn43 = getelementptr inbounds double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vtable42, i64 1
  %61 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vfn43
  %62 = load i32* %i, align 4
  %sub44 = sub i32 %62, 1
  %63 = load i32* %j, align 4
  %sub45 = sub i32 %63, 1
  %call46 = call double %61(%"class.boxmg::bmg2d::core::Array.72"* %59, i32 %sub44, i32 %sub45)
  %mul47 = fmul double %call41, %call46
  %add48 = fadd double %add40, %mul47
  %64 = load %"class.boxmg::bmg2d::core::GridStencil"** %so, align 8
  %65 = load i32* %i, align 4
  %66 = load i32* %j, align 4
  %call49 = call double @_ZNK5boxmg5bmg2d4core11GridStencilclEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %64, i32 %65, i32 %66, i32 5)
  %67 = load %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  %68 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %67 to double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)***
  %vtable50 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)*** %68
  %vfn51 = getelementptr inbounds double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vtable50, i64 1
  %69 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vfn51
  %70 = load i32* %i, align 4
  %add52 = add i32 %70, 1
  %71 = load i32* %j, align 4
  %sub53 = sub i32 %71, 1
  %call54 = call double %69(%"class.boxmg::bmg2d::core::Array.72"* %67, i32 %add52, i32 %sub53)
  %mul55 = fmul double %call49, %call54
  %add56 = fadd double %add48, %mul55
  %72 = load %"class.boxmg::bmg2d::core::GridStencil"** %so, align 8
  %73 = load i32* %i, align 4
  %74 = load i32* %j, align 4
  %call57 = call double @_ZNK5boxmg5bmg2d4core11GridStencilclEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %72, i32 %73, i32 %74, i32 4)
  %75 = load %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  %76 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %75 to double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)***
  %vtable58 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)*** %76
  %vfn59 = getelementptr inbounds double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vtable58, i64 1
  %77 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vfn59
  %78 = load i32* %i, align 4
  %sub60 = sub i32 %78, 1
  %79 = load i32* %j, align 4
  %add61 = add i32 %79, 1
  %call62 = call double %77(%"class.boxmg::bmg2d::core::Array.72"* %75, i32 %sub60, i32 %add61)
  %mul63 = fmul double %call57, %call62
  %add64 = fadd double %add56, %mul63
  %80 = load %"class.boxmg::bmg2d::core::GridStencil"** %so, align 8
  %81 = load i32* %i, align 4
  %82 = load i32* %j, align 4
  %call65 = call double @_ZNK5boxmg5bmg2d4core11GridStencilclEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %80, i32 %81, i32 %82, i32 7)
  %83 = load %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  %84 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %83 to double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)***
  %vtable66 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)*** %84
  %vfn67 = getelementptr inbounds double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vtable66, i64 1
  %85 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vfn67
  %86 = load i32* %i, align 4
  %add68 = add i32 %86, 1
  %87 = load i32* %j, align 4
  %add69 = add i32 %87, 1
  %call70 = call double %85(%"class.boxmg::bmg2d::core::Array.72"* %83, i32 %add68, i32 %add69)
  %mul71 = fmul double %call65, %call70
  %add72 = fadd double %add64, %mul71
  %88 = load %"class.boxmg::bmg2d::core::GridStencil"** %so, align 8
  %89 = load i32* %i, align 4
  %90 = load i32* %j, align 4
  %call73 = call double @_ZNK5boxmg5bmg2d4core11GridStencilclEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %88, i32 %89, i32 %90, i32 0)
  %91 = load %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  %92 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %91 to double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)***
  %vtable74 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)*** %92
  %vfn75 = getelementptr inbounds double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vtable74, i64 1
  %93 = load double (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vfn75
  %94 = load i32* %i, align 4
  %95 = load i32* %j, align 4
  %call76 = call double %93(%"class.boxmg::bmg2d::core::Array.72"* %91, i32 %94, i32 %95)
  %mul77 = fmul double %call73, %call76
  %sub78 = fsub double %add72, %mul77
  %add79 = fadd double %call16, %sub78
  %96 = load %"class.boxmg::bmg2d::core::Array.72"** %r.addr, align 8
  %97 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %96 to double* (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)***
  %vtable80 = load double* (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)*** %97
  %vfn81 = getelementptr inbounds double* (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vtable80, i64 0
  %98 = load double* (%"class.boxmg::bmg2d::core::Array.72"*, i32, i32)** %vfn81
  %99 = load i32* %i, align 4
  %100 = load i32* %j, align 4
  %call82 = call dereferenceable(8) double* %98(%"class.boxmg::bmg2d::core::Array.72"* %96, i32 %99, i32 %100)
  store double %add79, double* %call82
  br label %for.inc

for.inc:                                          ; preds = %for.body12
  %101 = bitcast %"class.boost::counting_iterator"* %__begin8 to %"class.boost::iterator_facade"*
  %call83 = call dereferenceable(4) %"class.boost::counting_iterator"* @_ZN5boost15iterator_facadeINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlEppEv(%"class.boost::iterator_facade"* %101)
  br label %for.cond10

for.end:                                          ; preds = %for.cond10
  br label %for.inc84

for.inc84:                                        ; preds = %for.end
  %102 = bitcast %"class.boost::counting_iterator"* %__begin to %"class.boost::iterator_facade"*
  %call85 = call dereferenceable(4) %"class.boost::counting_iterator"* @_ZN5boost15iterator_facadeINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlEppEv(%"class.boost::iterator_facade"* %102)
  br label %for.cond

for.end86:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr dereferenceable(112) %"class.boxmg::bmg2d::core::GridStencil"* @_ZNK5boxmg5bmg2d4core9StencilOp7stencilEv(%"class.boxmg::bmg2d::core::StencilOp"* %this) #5 align 2 {
entry:
  %this.addr = alloca %"class.boxmg::bmg2d::core::StencilOp"*, align 8
  store %"class.boxmg::bmg2d::core::StencilOp"* %this, %"class.boxmg::bmg2d::core::StencilOp"** %this.addr, align 8
  %this1 = load %"class.boxmg::bmg2d::core::StencilOp"** %this.addr
  %gs = getelementptr inbounds %"class.boxmg::bmg2d::core::StencilOp"* %this1, i32 0, i32 1
  ret %"class.boxmg::bmg2d::core::GridStencil"* %gs
}

; Function Attrs: uwtable
define linkonce_odr void @_ZNK5boost14iterator_rangeINS_17counting_iteratorIjNS_11use_defaultES2_EEE5beginEv(%"class.boost::counting_iterator"* noalias sret %agg.result, %"class.boost::iterator_range"* %this) #6 align 2 {
entry:
  %this.addr = alloca %"class.boost::iterator_range"*, align 8
  store %"class.boost::iterator_range"* %this, %"class.boost::iterator_range"** %this.addr, align 8
  %this1 = load %"class.boost::iterator_range"** %this.addr
  %m_Begin = getelementptr inbounds %"class.boost::iterator_range"* %this1, i32 0, i32 0
  call void @_ZN5boost17counting_iteratorIjNS_11use_defaultES1_EC2ERKS2_(%"class.boost::counting_iterator"* %agg.result, %"class.boost::counting_iterator"* dereferenceable(4) %m_Begin)
  ret void
}

; Function Attrs: uwtable
define linkonce_odr void @_ZNK5boost14iterator_rangeINS_17counting_iteratorIjNS_11use_defaultES2_EEE3endEv(%"class.boost::counting_iterator"* noalias sret %agg.result, %"class.boost::iterator_range"* %this) #6 align 2 {
entry:
  %this.addr = alloca %"class.boost::iterator_range"*, align 8
  store %"class.boost::iterator_range"* %this, %"class.boost::iterator_range"** %this.addr, align 8
  %this1 = load %"class.boost::iterator_range"** %this.addr
  %m_End = getelementptr inbounds %"class.boost::iterator_range"* %this1, i32 0, i32 1
  call void @_ZN5boost17counting_iteratorIjNS_11use_defaultES1_EC2ERKS2_(%"class.boost::counting_iterator"* %agg.result, %"class.boost::counting_iterator"* dereferenceable(4) %m_End)
  ret void
}

; Function Attrs: inlinehint uwtable
define linkonce_odr zeroext i1 @_ZN5boostneINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlS3_jS4_S6_lEENS_6detail23enable_if_interoperableIT_T4_NS_3mpl6apply2INS7_12always_bool2ES9_SA_E4typeEE4typeERKNS_15iterator_facadeIS9_T0_T1_T2_T3_EERKNSI_ISA_T5_T6_T7_T8_EE(%"class.boost::iterator_facade"* dereferenceable(1) %lhs, %"class.boost::iterator_facade"* dereferenceable(1) %rhs) #7 {
entry:
  %lhs.addr = alloca %"class.boost::iterator_facade"*, align 8
  %rhs.addr = alloca %"class.boost::iterator_facade"*, align 8
  %agg.tmp = alloca %"struct.mpl_::bool_", align 1
  %ref.tmp = alloca %"struct.boost::is_convertible", align 1
  store %"class.boost::iterator_facade"* %lhs, %"class.boost::iterator_facade"** %lhs.addr, align 8
  store %"class.boost::iterator_facade"* %rhs, %"class.boost::iterator_facade"** %rhs.addr, align 8
  %0 = load %"class.boost::iterator_facade"** %lhs.addr, align 8
  %1 = bitcast %"class.boost::iterator_facade"* %0 to %"class.boost::counting_iterator"*
  %2 = load %"class.boost::iterator_facade"** %rhs.addr, align 8
  %3 = bitcast %"class.boost::iterator_facade"* %2 to %"class.boost::counting_iterator"*
  %4 = bitcast %"struct.boost::is_convertible"* %ref.tmp to %"struct.mpl_::bool_"*
  %call = call zeroext i1 @_ZN5boost20iterator_core_access5equalINS_17counting_iteratorIjNS_11use_defaultES3_EES4_EEbRKT_RKT0_N4mpl_5bool_ILb1EEE(%"class.boost::counting_iterator"* dereferenceable(4) %1, %"class.boost::counting_iterator"* dereferenceable(4) %3)
  %lnot = xor i1 %call, true
  ret i1 %lnot
}

; Function Attrs: uwtable
define linkonce_odr dereferenceable(4) i32* @_ZNK5boost15iterator_facadeINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlEdeEv(%"class.boost::iterator_facade"* %this) #6 align 2 {
entry:
  %this.addr = alloca %"class.boost::iterator_facade"*, align 8
  store %"class.boost::iterator_facade"* %this, %"class.boost::iterator_facade"** %this.addr, align 8
  %this1 = load %"class.boost::iterator_facade"** %this.addr
  %call = call dereferenceable(4) %"class.boost::counting_iterator"* @_ZNK5boost15iterator_facadeINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlE7derivedEv(%"class.boost::iterator_facade"* %this1)
  %call2 = call dereferenceable(4) i32* @_ZN5boost20iterator_core_access11dereferenceINS_17counting_iteratorIjNS_11use_defaultES3_EEEENT_9referenceERKS5_(%"class.boost::counting_iterator"* dereferenceable(4) %call)
  ret i32* %call2
}

; Function Attrs: uwtable
define linkonce_odr double @_ZNK5boxmg5bmg2d4core11GridStencilclEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %this, i32 %i, i32 %j, i32 %dir) #6 align 2 {
entry:
  %this.addr = alloca %"class.boxmg::bmg2d::core::GridStencil"*, align 8
  %i.addr = alloca i32, align 4
  %j.addr = alloca i32, align 4
  %dir.addr = alloca i32, align 4
  store %"class.boxmg::bmg2d::core::GridStencil"* %this, %"class.boxmg::bmg2d::core::GridStencil"** %this.addr, align 8
  store i32 %i, i32* %i.addr, align 4
  store i32 %j, i32* %j.addr, align 4
  store i32 %dir, i32* %dir.addr, align 4
  %this1 = load %"class.boxmg::bmg2d::core::GridStencil"** %this.addr
  call void @_ZNK5boxmg5bmg2d4core11GridStencil14symmetric_hackERjS3_RNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %this1, i32* dereferenceable(4) %i.addr, i32* dereferenceable(4) %j.addr, i32* dereferenceable(4) %dir.addr)
  %0 = load i32* %i.addr, align 4
  %1 = load i32* %j.addr, align 4
  %2 = load i32* %dir.addr, align 4
  %call = call i32 @_ZNK5boxmg5bmg2d4core11GridStencil5indexEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %this1, i32 %0, i32 %1, i32 %2)
  %idxprom = zext i32 %call to i64
  %ddata_ = getelementptr inbounds %"class.boxmg::bmg2d::core::GridStencil"* %this1, i32 0, i32 3
  %call2 = call double* @_ZNKSt6vectorId14AlignAllocatorIdLm16EEE4dataEv(%"class.std::vector.68"* %ddata_) #2
  %arrayidx = getelementptr inbounds double* %call2, i64 %idxprom
  %3 = load double* %arrayidx, align 8
  ret double %3
}

; Function Attrs: uwtable
define linkonce_odr dereferenceable(4) %"class.boost::counting_iterator"* @_ZN5boost15iterator_facadeINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlEppEv(%"class.boost::iterator_facade"* %this) #6 align 2 {
entry:
  %this.addr = alloca %"class.boost::iterator_facade"*, align 8
  store %"class.boost::iterator_facade"* %this, %"class.boost::iterator_facade"** %this.addr, align 8
  %this1 = load %"class.boost::iterator_facade"** %this.addr
  %call = call dereferenceable(4) %"class.boost::counting_iterator"* @_ZN5boost15iterator_facadeINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlE7derivedEv(%"class.boost::iterator_facade"* %this1)
  call void @_ZN5boost20iterator_core_access9incrementINS_17counting_iteratorIjNS_11use_defaultES3_EEEEvRT_(%"class.boost::counting_iterator"* dereferenceable(4) %call)
  %call2 = call dereferenceable(4) %"class.boost::counting_iterator"* @_ZN5boost15iterator_facadeINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlE7derivedEv(%"class.boost::iterator_facade"* %this1)
  ret %"class.boost::counting_iterator"* %call2
}

; Function Attrs: uwtable
define void @_ZN5boxmg5bmg2d6kernel5impls16residual_fortranERKNS0_4core9StencilOpERKNS3_5ArrayIjdEESA_RS8_(%"class.boxmg::bmg2d::core::StencilOp"* dereferenceable(120) %A, %"class.boxmg::bmg2d::core::Array.72"* dereferenceable(88) %x, %"class.boxmg::bmg2d::core::Array.72"* dereferenceable(88) %b, %"class.boxmg::bmg2d::core::Array.72"* dereferenceable(88) %r) #6 {
entry:
  %A.addr = alloca %"class.boxmg::bmg2d::core::StencilOp"*, align 8
  %x.addr = alloca %"class.boxmg::bmg2d::core::Array.72"*, align 8
  %b.addr = alloca %"class.boxmg::bmg2d::core::Array.72"*, align 8
  %r.addr = alloca %"class.boxmg::bmg2d::core::Array.72"*, align 8
  %k = alloca i32, align 4
  %kf = alloca i32, align 4
  %ifd = alloca i32, align 4
  %nstncl = alloca i32, align 4
  %ibc = alloca i32, align 4
  %irelax = alloca i32, align 4
  %irelax_sym = alloca i32, align 4
  %updown = alloca i32, align 4
  %ii = alloca i32, align 4
  %jj = alloca i32, align 4
  %Ad = alloca %"class.boxmg::bmg2d::core::StencilOp"*, align 8
  %xd = alloca %"class.boxmg::bmg2d::core::Array.72"*, align 8
  %bd = alloca %"class.boxmg::bmg2d::core::Array.72"*, align 8
  store %"class.boxmg::bmg2d::core::StencilOp"* %A, %"class.boxmg::bmg2d::core::StencilOp"** %A.addr, align 8
  store %"class.boxmg::bmg2d::core::Array.72"* %x, %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  store %"class.boxmg::bmg2d::core::Array.72"* %b, %"class.boxmg::bmg2d::core::Array.72"** %b.addr, align 8
  store %"class.boxmg::bmg2d::core::Array.72"* %r, %"class.boxmg::bmg2d::core::Array.72"** %r.addr, align 8
  store i32 0, i32* %k, align 4
  store i32 0, i32* %kf, align 4
  store i32 0, i32* %ifd, align 4
  store i32 5, i32* %nstncl, align 4
  store i32 0, i32* %ibc, align 4
  store i32 0, i32* %irelax, align 4
  store i32 0, i32* %irelax_sym, align 4
  store i32 0, i32* %updown, align 4
  %0 = load %"class.boxmg::bmg2d::core::Array.72"** %r.addr, align 8
  %1 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %0 to i32 (%"class.boxmg::bmg2d::core::Array.72"*, i32)***
  %vtable = load i32 (%"class.boxmg::bmg2d::core::Array.72"*, i32)*** %1
  %vfn = getelementptr inbounds i32 (%"class.boxmg::bmg2d::core::Array.72"*, i32)** %vtable, i64 2
  %2 = load i32 (%"class.boxmg::bmg2d::core::Array.72"*, i32)** %vfn
  %call = call i32 %2(%"class.boxmg::bmg2d::core::Array.72"* %0, i32 0)
  store i32 %call, i32* %ii, align 4
  %3 = load %"class.boxmg::bmg2d::core::Array.72"** %r.addr, align 8
  %4 = bitcast %"class.boxmg::bmg2d::core::Array.72"* %3 to i32 (%"class.boxmg::bmg2d::core::Array.72"*, i32)***
  %vtable1 = load i32 (%"class.boxmg::bmg2d::core::Array.72"*, i32)*** %4
  %vfn2 = getelementptr inbounds i32 (%"class.boxmg::bmg2d::core::Array.72"*, i32)** %vtable1, i64 2
  %5 = load i32 (%"class.boxmg::bmg2d::core::Array.72"*, i32)** %vfn2
  %call3 = call i32 %5(%"class.boxmg::bmg2d::core::Array.72"* %3, i32 1)
  store i32 %call3, i32* %jj, align 4
  %6 = load %"class.boxmg::bmg2d::core::StencilOp"** %A.addr, align 8
  store %"class.boxmg::bmg2d::core::StencilOp"* %6, %"class.boxmg::bmg2d::core::StencilOp"** %Ad, align 8
  %7 = load %"class.boxmg::bmg2d::core::Array.72"** %x.addr, align 8
  store %"class.boxmg::bmg2d::core::Array.72"* %7, %"class.boxmg::bmg2d::core::Array.72"** %xd, align 8
  %8 = load %"class.boxmg::bmg2d::core::Array.72"** %b.addr, align 8
  store %"class.boxmg::bmg2d::core::Array.72"* %8, %"class.boxmg::bmg2d::core::Array.72"** %bd, align 8
  %9 = load %"class.boxmg::bmg2d::core::StencilOp"** %Ad, align 8
  %call4 = call double* @_ZN5boxmg5bmg2d4core9StencilOp4dataEv(%"class.boxmg::bmg2d::core::StencilOp"* %9)
  %10 = load %"class.boxmg::bmg2d::core::Array.72"** %bd, align 8
  %call5 = call double* @_ZN5boxmg5bmg2d4core5ArrayIjdE4dataEv(%"class.boxmg::bmg2d::core::Array.72"* %10)
  %11 = load %"class.boxmg::bmg2d::core::Array.72"** %xd, align 8
  %call6 = call double* @_ZN5boxmg5bmg2d4core5ArrayIjdE4dataEv(%"class.boxmg::bmg2d::core::Array.72"* %11)
  %12 = load %"class.boxmg::bmg2d::core::Array.72"** %r.addr, align 8
  %call7 = call double* @_ZN5boxmg5bmg2d4core5ArrayIjdE4dataEv(%"class.boxmg::bmg2d::core::Array.72"* %12)
  call void @bmg2_symstd_residual(i32* %k, double* %call4, double* %call5, double* %call6, double* %call7, i32* %ii, i32* %jj, i32* %kf, i32* %ifd, i32* %nstncl, i32* %ibc, i32* %irelax, i32* %irelax_sym, i32* %updown)
  ret void
}

declare void @bmg2_symstd_residual(i32*, double*, double*, double*, double*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, i32*) #0

; Function Attrs: uwtable
define linkonce_odr double* @_ZN5boxmg5bmg2d4core9StencilOp4dataEv(%"class.boxmg::bmg2d::core::StencilOp"* %this) #6 align 2 {
entry:
  %this.addr = alloca %"class.boxmg::bmg2d::core::StencilOp"*, align 8
  store %"class.boxmg::bmg2d::core::StencilOp"* %this, %"class.boxmg::bmg2d::core::StencilOp"** %this.addr, align 8
  %this1 = load %"class.boxmg::bmg2d::core::StencilOp"** %this.addr
  %gs = getelementptr inbounds %"class.boxmg::bmg2d::core::StencilOp"* %this1, i32 0, i32 1
  %call = call double* @_ZN5boxmg5bmg2d4core11GridStencil4dataEv(%"class.boxmg::bmg2d::core::GridStencil"* %gs)
  ret double* %call
}

; Function Attrs: nounwind uwtable
define linkonce_odr double* @_ZN5boxmg5bmg2d4core5ArrayIjdE4dataEv(%"class.boxmg::bmg2d::core::Array.72"* %this) #5 align 2 {
entry:
  %this.addr = alloca %"class.boxmg::bmg2d::core::Array.72"*, align 8
  store %"class.boxmg::bmg2d::core::Array.72"* %this, %"class.boxmg::bmg2d::core::Array.72"** %this.addr, align 8
  %this1 = load %"class.boxmg::bmg2d::core::Array.72"** %this.addr
  %data_ = getelementptr inbounds %"class.boxmg::bmg2d::core::Array.72"* %this1, i32 0, i32 1
  %call = call double* @_ZNSt6vectorId14AlignAllocatorIdLm16EEE4dataEv(%"class.std::vector.68"* %data_) #2
  ret double* %call
}

; Function Attrs: nounwind
declare i8* @_ZNKSt13runtime_error4whatEv(%"class.std::runtime_error"*) #1

; Function Attrs: nounwind uwtable
define linkonce_odr double* @_ZNSt6vectorId14AlignAllocatorIdLm16EEE4dataEv(%"class.std::vector.68"* %this) #5 align 2 {
entry:
  %this.addr = alloca %"class.std::vector.68"*, align 8
  store %"class.std::vector.68"* %this, %"class.std::vector.68"** %this.addr, align 8
  %this1 = load %"class.std::vector.68"** %this.addr
  %0 = bitcast %"class.std::vector.68"* %this1 to %"struct.std::_Vector_base.69"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base.69"* %0, i32 0, i32 0
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<double, AlignAllocator<double, 16> >::_Vector_impl"* %_M_impl, i32 0, i32 0
  %1 = load double** %_M_start, align 8
  %call = invoke double* @_ZNKSt6vectorId14AlignAllocatorIdLm16EEE11_M_data_ptrIdEEPT_S5_(%"class.std::vector.68"* %this1, double* %1)
          to label %invoke.cont unwind label %terminate.lpad

invoke.cont:                                      ; preds = %entry
  ret double* %call

terminate.lpad:                                   ; preds = %entry
  %2 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*)
          catch i8* null
  %3 = extractvalue { i8*, i32 } %2, 0
  call void @__clang_call_terminate(i8* %3) #10
  unreachable
}

; Function Attrs: nounwind uwtable
define linkonce_odr double* @_ZNKSt6vectorId14AlignAllocatorIdLm16EEE11_M_data_ptrIdEEPT_S5_(%"class.std::vector.68"* %this, double* %__ptr) #5 align 2 {
entry:
  %this.addr = alloca %"class.std::vector.68"*, align 8
  %__ptr.addr = alloca double*, align 8
  store %"class.std::vector.68"* %this, %"class.std::vector.68"** %this.addr, align 8
  store double* %__ptr, double** %__ptr.addr, align 8
  %this1 = load %"class.std::vector.68"** %this.addr
  %0 = load double** %__ptr.addr, align 8
  ret double* %0
}

declare i32 @__gxx_personality_v0(...)

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #8 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #2
  call void @_ZSt9terminatev() #10
  unreachable
}

declare i8* @__cxa_begin_catch(i8*)

declare void @_ZSt9terminatev()

; Function Attrs: uwtable
define linkonce_odr dereferenceable(4) i32* @_ZN5boost20iterator_core_access11dereferenceINS_17counting_iteratorIjNS_11use_defaultES3_EEEENT_9referenceERKS5_(%"class.boost::counting_iterator"* dereferenceable(4) %f) #6 align 2 {
entry:
  %f.addr = alloca %"class.boost::counting_iterator"*, align 8
  store %"class.boost::counting_iterator"* %f, %"class.boost::counting_iterator"** %f.addr, align 8
  %0 = load %"class.boost::counting_iterator"** %f.addr, align 8
  %call = call dereferenceable(4) i32* @_ZNK5boost17counting_iteratorIjNS_11use_defaultES1_E11dereferenceEv(%"class.boost::counting_iterator"* %0)
  ret i32* %call
}

; Function Attrs: nounwind uwtable
define linkonce_odr dereferenceable(4) %"class.boost::counting_iterator"* @_ZNK5boost15iterator_facadeINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlE7derivedEv(%"class.boost::iterator_facade"* %this) #5 align 2 {
entry:
  %this.addr = alloca %"class.boost::iterator_facade"*, align 8
  store %"class.boost::iterator_facade"* %this, %"class.boost::iterator_facade"** %this.addr, align 8
  %this1 = load %"class.boost::iterator_facade"** %this.addr
  %0 = bitcast %"class.boost::iterator_facade"* %this1 to %"class.boost::counting_iterator"*
  ret %"class.boost::counting_iterator"* %0
}

; Function Attrs: uwtable
define linkonce_odr dereferenceable(4) i32* @_ZNK5boost17counting_iteratorIjNS_11use_defaultES1_E11dereferenceEv(%"class.boost::counting_iterator"* %this) #6 align 2 {
entry:
  %this.addr = alloca %"class.boost::counting_iterator"*, align 8
  store %"class.boost::counting_iterator"* %this, %"class.boost::counting_iterator"** %this.addr, align 8
  %this1 = load %"class.boost::counting_iterator"** %this.addr
  %0 = bitcast %"class.boost::counting_iterator"* %this1 to %"class.boost::iterator_adaptor"*
  %call = call dereferenceable(4) i32* @_ZNK5boost16iterator_adaptorINS_17counting_iteratorIjNS_11use_defaultES2_EEjjNS_27random_access_traversal_tagERKjlE14base_referenceEv(%"class.boost::iterator_adaptor"* %0)
  ret i32* %call
}

; Function Attrs: nounwind uwtable
define linkonce_odr dereferenceable(4) i32* @_ZNK5boost16iterator_adaptorINS_17counting_iteratorIjNS_11use_defaultES2_EEjjNS_27random_access_traversal_tagERKjlE14base_referenceEv(%"class.boost::iterator_adaptor"* %this) #5 align 2 {
entry:
  %this.addr = alloca %"class.boost::iterator_adaptor"*, align 8
  store %"class.boost::iterator_adaptor"* %this, %"class.boost::iterator_adaptor"** %this.addr, align 8
  %this1 = load %"class.boost::iterator_adaptor"** %this.addr
  %m_iterator = getelementptr inbounds %"class.boost::iterator_adaptor"* %this1, i32 0, i32 0
  ret i32* %m_iterator
}

; Function Attrs: uwtable
define linkonce_odr void @_ZN5boost20iterator_core_access9incrementINS_17counting_iteratorIjNS_11use_defaultES3_EEEEvRT_(%"class.boost::counting_iterator"* dereferenceable(4) %f) #6 align 2 {
entry:
  %f.addr = alloca %"class.boost::counting_iterator"*, align 8
  store %"class.boost::counting_iterator"* %f, %"class.boost::counting_iterator"** %f.addr, align 8
  %0 = load %"class.boost::counting_iterator"** %f.addr, align 8
  %1 = bitcast %"class.boost::counting_iterator"* %0 to %"class.boost::iterator_adaptor"*
  call void @_ZN5boost16iterator_adaptorINS_17counting_iteratorIjNS_11use_defaultES2_EEjjNS_27random_access_traversal_tagERKjlE9incrementEv(%"class.boost::iterator_adaptor"* %1)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr dereferenceable(4) %"class.boost::counting_iterator"* @_ZN5boost15iterator_facadeINS_17counting_iteratorIjNS_11use_defaultES2_EEjNS_27random_access_traversal_tagERKjlE7derivedEv(%"class.boost::iterator_facade"* %this) #5 align 2 {
entry:
  %this.addr = alloca %"class.boost::iterator_facade"*, align 8
  store %"class.boost::iterator_facade"* %this, %"class.boost::iterator_facade"** %this.addr, align 8
  %this1 = load %"class.boost::iterator_facade"** %this.addr
  %0 = bitcast %"class.boost::iterator_facade"* %this1 to %"class.boost::counting_iterator"*
  ret %"class.boost::counting_iterator"* %0
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost16iterator_adaptorINS_17counting_iteratorIjNS_11use_defaultES2_EEjjNS_27random_access_traversal_tagERKjlE9incrementEv(%"class.boost::iterator_adaptor"* %this) #5 align 2 {
entry:
  %this.addr = alloca %"class.boost::iterator_adaptor"*, align 8
  store %"class.boost::iterator_adaptor"* %this, %"class.boost::iterator_adaptor"** %this.addr, align 8
  %this1 = load %"class.boost::iterator_adaptor"** %this.addr
  %m_iterator = getelementptr inbounds %"class.boost::iterator_adaptor"* %this1, i32 0, i32 0
  %0 = load i32* %m_iterator, align 4
  %inc = add i32 %0, 1
  store i32 %inc, i32* %m_iterator, align 4
  ret void
}

; Function Attrs: uwtable
define linkonce_odr zeroext i1 @_ZN5boost20iterator_core_access5equalINS_17counting_iteratorIjNS_11use_defaultES3_EES4_EEbRKT_RKT0_N4mpl_5bool_ILb1EEE(%"class.boost::counting_iterator"* dereferenceable(4) %f1, %"class.boost::counting_iterator"* dereferenceable(4) %f2) #6 align 2 {
entry:
  %0 = alloca %"struct.mpl_::bool_", align 1
  %f1.addr = alloca %"class.boost::counting_iterator"*, align 8
  %f2.addr = alloca %"class.boost::counting_iterator"*, align 8
  store %"class.boost::counting_iterator"* %f1, %"class.boost::counting_iterator"** %f1.addr, align 8
  store %"class.boost::counting_iterator"* %f2, %"class.boost::counting_iterator"** %f2.addr, align 8
  %1 = load %"class.boost::counting_iterator"** %f1.addr, align 8
  %2 = bitcast %"class.boost::counting_iterator"* %1 to %"class.boost::iterator_adaptor"*
  %3 = load %"class.boost::counting_iterator"** %f2.addr, align 8
  %4 = bitcast %"class.boost::counting_iterator"* %3 to %"class.boost::iterator_adaptor"*
  %call = call zeroext i1 @_ZNK5boost16iterator_adaptorINS_17counting_iteratorIjNS_11use_defaultES2_EEjjNS_27random_access_traversal_tagERKjlE5equalIS3_jjS4_S6_lEEbRKNS0_IT_T0_T1_T2_T3_T4_EE(%"class.boost::iterator_adaptor"* %2, %"class.boost::iterator_adaptor"* dereferenceable(4) %4)
  ret i1 %call
}

; Function Attrs: uwtable
define linkonce_odr zeroext i1 @_ZNK5boost16iterator_adaptorINS_17counting_iteratorIjNS_11use_defaultES2_EEjjNS_27random_access_traversal_tagERKjlE5equalIS3_jjS4_S6_lEEbRKNS0_IT_T0_T1_T2_T3_T4_EE(%"class.boost::iterator_adaptor"* %this, %"class.boost::iterator_adaptor"* dereferenceable(4) %x) #6 align 2 {
entry:
  %this.addr = alloca %"class.boost::iterator_adaptor"*, align 8
  %x.addr = alloca %"class.boost::iterator_adaptor"*, align 8
  store %"class.boost::iterator_adaptor"* %this, %"class.boost::iterator_adaptor"** %this.addr, align 8
  store %"class.boost::iterator_adaptor"* %x, %"class.boost::iterator_adaptor"** %x.addr, align 8
  %this1 = load %"class.boost::iterator_adaptor"** %this.addr
  %m_iterator = getelementptr inbounds %"class.boost::iterator_adaptor"* %this1, i32 0, i32 0
  %0 = load i32* %m_iterator, align 4
  %1 = load %"class.boost::iterator_adaptor"** %x.addr, align 8
  %call = call dereferenceable(4) i32* @_ZNK5boost16iterator_adaptorINS_17counting_iteratorIjNS_11use_defaultES2_EEjjNS_27random_access_traversal_tagERKjlE4baseEv(%"class.boost::iterator_adaptor"* %1)
  %2 = load i32* %call
  %cmp = icmp eq i32 %0, %2
  ret i1 %cmp
}

; Function Attrs: nounwind uwtable
define linkonce_odr dereferenceable(4) i32* @_ZNK5boost16iterator_adaptorINS_17counting_iteratorIjNS_11use_defaultES2_EEjjNS_27random_access_traversal_tagERKjlE4baseEv(%"class.boost::iterator_adaptor"* %this) #5 align 2 {
entry:
  %this.addr = alloca %"class.boost::iterator_adaptor"*, align 8
  store %"class.boost::iterator_adaptor"* %this, %"class.boost::iterator_adaptor"** %this.addr, align 8
  %this1 = load %"class.boost::iterator_adaptor"** %this.addr
  %m_iterator = getelementptr inbounds %"class.boost::iterator_adaptor"* %this1, i32 0, i32 0
  ret i32* %m_iterator
}

; Function Attrs: uwtable
define linkonce_odr void @_ZN5boost17counting_iteratorIjNS_11use_defaultES1_EC2ERKS2_(%"class.boost::counting_iterator"* %this, %"class.boost::counting_iterator"* dereferenceable(4) %rhs) unnamed_addr #6 align 2 {
entry:
  %this.addr = alloca %"class.boost::counting_iterator"*, align 8
  %rhs.addr = alloca %"class.boost::counting_iterator"*, align 8
  store %"class.boost::counting_iterator"* %this, %"class.boost::counting_iterator"** %this.addr, align 8
  store %"class.boost::counting_iterator"* %rhs, %"class.boost::counting_iterator"** %rhs.addr, align 8
  %this1 = load %"class.boost::counting_iterator"** %this.addr
  %0 = bitcast %"class.boost::counting_iterator"* %this1 to %"class.boost::iterator_adaptor"*
  %1 = load %"class.boost::counting_iterator"** %rhs.addr, align 8
  %2 = bitcast %"class.boost::counting_iterator"* %1 to %"class.boost::iterator_adaptor"*
  %call = call dereferenceable(4) i32* @_ZNK5boost16iterator_adaptorINS_17counting_iteratorIjNS_11use_defaultES2_EEjjNS_27random_access_traversal_tagERKjlE4baseEv(%"class.boost::iterator_adaptor"* %2)
  call void @_ZN5boost16iterator_adaptorINS_17counting_iteratorIjNS_11use_defaultES2_EEjjNS_27random_access_traversal_tagERKjlEC2ES6_(%"class.boost::iterator_adaptor"* %0, i32* dereferenceable(4) %call)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost16iterator_adaptorINS_17counting_iteratorIjNS_11use_defaultES2_EEjjNS_27random_access_traversal_tagERKjlEC2ES6_(%"class.boost::iterator_adaptor"* %this, i32* dereferenceable(4) %iter) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"class.boost::iterator_adaptor"*, align 8
  %iter.addr = alloca i32*, align 8
  store %"class.boost::iterator_adaptor"* %this, %"class.boost::iterator_adaptor"** %this.addr, align 8
  store i32* %iter, i32** %iter.addr, align 8
  %this1 = load %"class.boost::iterator_adaptor"** %this.addr
  %0 = bitcast %"class.boost::iterator_adaptor"* %this1 to %"class.boost::iterator_facade"*
  %m_iterator = getelementptr inbounds %"class.boost::iterator_adaptor"* %this1, i32 0, i32 0
  %1 = load i32** %iter.addr, align 8
  %2 = load i32* %1, align 4
  store i32 %2, i32* %m_iterator, align 4
  ret void
}

; Function Attrs: inlinehint uwtable
define linkonce_odr void @_ZN5boost6spirit7classicdeINS1_14anychar_parserEEENS1_11kleene_starIT_EERKNS1_6parserIS5_EE(%"struct.boost::spirit::classic::parser.16"* dereferenceable(1) %a) #7 {
entry:
  %retval = alloca %"struct.boost::spirit::classic::kleene_star", align 1
  %a.addr = alloca %"struct.boost::spirit::classic::parser.16"*, align 8
  store %"struct.boost::spirit::classic::parser.16"* %a, %"struct.boost::spirit::classic::parser.16"** %a.addr, align 8
  %0 = load %"struct.boost::spirit::classic::parser.16"** %a.addr, align 8
  %call = call dereferenceable(1) %"struct.boost::spirit::classic::anychar_parser"* @_ZNK5boost6spirit7classic6parserINS1_14anychar_parserEE7derivedEv(%"struct.boost::spirit::classic::parser.16"* %0)
  call void @_ZN5boost6spirit7classic11kleene_starINS1_14anychar_parserEEC2ERKS3_(%"struct.boost::spirit::classic::kleene_star"* %retval, %"struct.boost::spirit::classic::anychar_parser"* dereferenceable(1) %call)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic5chlitIcEC2Ec(%"struct.boost::spirit::classic::chlit"* %this, i8 signext %ch_) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::chlit"*, align 8
  %ch_.addr = alloca i8, align 1
  store %"struct.boost::spirit::classic::chlit"* %this, %"struct.boost::spirit::classic::chlit"** %this.addr, align 8
  store i8 %ch_, i8* %ch_.addr, align 1
  %this1 = load %"struct.boost::spirit::classic::chlit"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::chlit"* %this1 to %"struct.boost::spirit::classic::char_parser.54"*
  %ch = getelementptr inbounds %"struct.boost::spirit::classic::chlit"* %this1, i32 0, i32 0
  %1 = load i8* %ch_.addr, align 1
  store i8 %1, i8* %ch, align 1
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic11list_parserINS1_11kleene_starINS1_14anychar_parserEEENS1_5chlitIcEENS1_16no_list_endtokenENS1_21plain_parser_categoryEEC2ERKS5_RKS7_RKS8_(%"struct.boost::spirit::classic::list_parser"* %this, %"struct.boost::spirit::classic::kleene_star"* dereferenceable(1) %item_, %"struct.boost::spirit::classic::chlit"* dereferenceable(1) %delim_, %"struct.boost::spirit::classic::no_list_endtoken"* dereferenceable(1) %end_) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::list_parser"*, align 8
  %item_.addr = alloca %"struct.boost::spirit::classic::kleene_star"*, align 8
  %delim_.addr = alloca %"struct.boost::spirit::classic::chlit"*, align 8
  %end_.addr = alloca %"struct.boost::spirit::classic::no_list_endtoken"*, align 8
  store %"struct.boost::spirit::classic::list_parser"* %this, %"struct.boost::spirit::classic::list_parser"** %this.addr, align 8
  store %"struct.boost::spirit::classic::kleene_star"* %item_, %"struct.boost::spirit::classic::kleene_star"** %item_.addr, align 8
  store %"struct.boost::spirit::classic::chlit"* %delim_, %"struct.boost::spirit::classic::chlit"** %delim_.addr, align 8
  store %"struct.boost::spirit::classic::no_list_endtoken"* %end_, %"struct.boost::spirit::classic::no_list_endtoken"** %end_.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::list_parser"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::list_parser"* %this1 to %"struct.boost::spirit::classic::parser.52"*
  %item = getelementptr inbounds %"struct.boost::spirit::classic::list_parser"* %this1, i32 0, i32 0
  %1 = load %"struct.boost::spirit::classic::kleene_star"** %item_.addr, align 8
  %2 = bitcast %"struct.boost::spirit::classic::kleene_star"* %item to i8*
  %3 = bitcast %"struct.boost::spirit::classic::kleene_star"* %1 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 1, i32 1, i1 false)
  %delim = getelementptr inbounds %"struct.boost::spirit::classic::list_parser"* %this1, i32 0, i32 1
  %4 = load %"struct.boost::spirit::classic::chlit"** %delim_.addr, align 8
  %5 = bitcast %"struct.boost::spirit::classic::chlit"* %delim to i8*
  %6 = bitcast %"struct.boost::spirit::classic::chlit"* %4 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %5, i8* %6, i64 1, i32 1, i1 false)
  %end = getelementptr inbounds %"struct.boost::spirit::classic::list_parser"* %this1, i32 0, i32 2
  %7 = load %"struct.boost::spirit::classic::no_list_endtoken"** %end_.addr, align 8
  ret void
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #2

; Function Attrs: nounwind uwtable
define linkonce_odr dereferenceable(1) %"struct.boost::spirit::classic::anychar_parser"* @_ZNK5boost6spirit7classic6parserINS1_14anychar_parserEE7derivedEv(%"struct.boost::spirit::classic::parser.16"* %this) #5 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::parser.16"*, align 8
  store %"struct.boost::spirit::classic::parser.16"* %this, %"struct.boost::spirit::classic::parser.16"** %this.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::parser.16"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::parser.16"* %this1 to %"struct.boost::spirit::classic::anychar_parser"*
  ret %"struct.boost::spirit::classic::anychar_parser"* %0
}

; Function Attrs: uwtable
define linkonce_odr void @_ZN5boost6spirit7classic11kleene_starINS1_14anychar_parserEEC2ERKS3_(%"struct.boost::spirit::classic::kleene_star"* %this, %"struct.boost::spirit::classic::anychar_parser"* dereferenceable(1) %a) unnamed_addr #6 align 2 {
entry:
  %this.addr = alloca %"struct.boost::spirit::classic::kleene_star"*, align 8
  %a.addr = alloca %"struct.boost::spirit::classic::anychar_parser"*, align 8
  store %"struct.boost::spirit::classic::kleene_star"* %this, %"struct.boost::spirit::classic::kleene_star"** %this.addr, align 8
  store %"struct.boost::spirit::classic::anychar_parser"* %a, %"struct.boost::spirit::classic::anychar_parser"** %a.addr, align 8
  %this1 = load %"struct.boost::spirit::classic::kleene_star"** %this.addr
  %0 = bitcast %"struct.boost::spirit::classic::kleene_star"* %this1 to %"class.boost::spirit::classic::unary"*
  %1 = load %"struct.boost::spirit::classic::anychar_parser"** %a.addr, align 8
  call void @_ZN5boost6spirit7classic5unaryINS1_14anychar_parserENS1_6parserINS1_11kleene_starIS3_EEEEEC2ERKS3_(%"class.boost::spirit::classic::unary"* %0, %"struct.boost::spirit::classic::anychar_parser"* dereferenceable(1) %1)
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZN5boost6spirit7classic5unaryINS1_14anychar_parserENS1_6parserINS1_11kleene_starIS3_EEEEEC2ERKS3_(%"class.boost::spirit::classic::unary"* %this, %"struct.boost::spirit::classic::anychar_parser"* dereferenceable(1) %subj_) unnamed_addr #5 align 2 {
entry:
  %this.addr = alloca %"class.boost::spirit::classic::unary"*, align 8
  %subj_.addr = alloca %"struct.boost::spirit::classic::anychar_parser"*, align 8
  store %"class.boost::spirit::classic::unary"* %this, %"class.boost::spirit::classic::unary"** %this.addr, align 8
  store %"struct.boost::spirit::classic::anychar_parser"* %subj_, %"struct.boost::spirit::classic::anychar_parser"** %subj_.addr, align 8
  %this1 = load %"class.boost::spirit::classic::unary"** %this.addr
  %0 = bitcast %"class.boost::spirit::classic::unary"* %this1 to %"struct.boost::spirit::classic::parser.53"*
  %subj = getelementptr inbounds %"class.boost::spirit::classic::unary"* %this1, i32 0, i32 0
  %1 = load %"struct.boost::spirit::classic::anychar_parser"** %subj_.addr, align 8
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr double* @_ZN5boxmg5bmg2d4core11GridStencil4dataEv(%"class.boxmg::bmg2d::core::GridStencil"* %this) #5 align 2 {
entry:
  %this.addr = alloca %"class.boxmg::bmg2d::core::GridStencil"*, align 8
  store %"class.boxmg::bmg2d::core::GridStencil"* %this, %"class.boxmg::bmg2d::core::GridStencil"** %this.addr, align 8
  %this1 = load %"class.boxmg::bmg2d::core::GridStencil"** %this.addr
  %ddata_ = getelementptr inbounds %"class.boxmg::bmg2d::core::GridStencil"* %this1, i32 0, i32 3
  %call = call double* @_ZNSt6vectorId14AlignAllocatorIdLm16EEE4dataEv(%"class.std::vector.68"* %ddata_) #2
  ret double* %call
}

; Function Attrs: nounwind uwtable
define linkonce_odr void @_ZNK5boxmg5bmg2d4core11GridStencil14symmetric_hackERjS3_RNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %this, i32* dereferenceable(4) %i, i32* dereferenceable(4) %j, i32* dereferenceable(4) %dir) #5 align 2 {
entry:
  %this.addr = alloca %"class.boxmg::bmg2d::core::GridStencil"*, align 8
  %i.addr = alloca i32*, align 8
  %j.addr = alloca i32*, align 8
  %dir.addr = alloca i32*, align 8
  store %"class.boxmg::bmg2d::core::GridStencil"* %this, %"class.boxmg::bmg2d::core::GridStencil"** %this.addr, align 8
  store i32* %i, i32** %i.addr, align 8
  store i32* %j, i32** %j.addr, align 8
  store i32* %dir, i32** %dir.addr, align 8
  %this1 = load %"class.boxmg::bmg2d::core::GridStencil"** %this.addr
  %symmetric = getelementptr inbounds %"class.boxmg::bmg2d::core::GridStencil"* %this1, i32 0, i32 1
  %0 = load i8* %symmetric, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %land.lhs.true, label %if.end31

land.lhs.true:                                    ; preds = %entry
  %1 = load i32** %dir.addr, align 8
  %2 = load i32* %1, align 4
  %cmp = icmp eq i32 %2, 6
  br i1 %cmp, label %if.then, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %land.lhs.true
  %3 = load i32** %dir.addr, align 8
  %4 = load i32* %3, align 4
  %cmp2 = icmp eq i32 %4, 5
  br i1 %cmp2, label %if.then, label %lor.lhs.false3

lor.lhs.false3:                                   ; preds = %lor.lhs.false
  %5 = load i32** %dir.addr, align 8
  %6 = load i32* %5, align 4
  %cmp4 = icmp eq i32 %6, 7
  br i1 %cmp4, label %if.then, label %lor.lhs.false5

lor.lhs.false5:                                   ; preds = %lor.lhs.false3
  %7 = load i32** %dir.addr, align 8
  %8 = load i32* %7, align 4
  %cmp6 = icmp eq i32 %8, 8
  br i1 %cmp6, label %if.then, label %lor.lhs.false7

lor.lhs.false7:                                   ; preds = %lor.lhs.false5
  %9 = load i32** %dir.addr, align 8
  %10 = load i32* %9, align 4
  %cmp8 = icmp eq i32 %10, 4
  br i1 %cmp8, label %if.then, label %if.end31

if.then:                                          ; preds = %lor.lhs.false7, %lor.lhs.false5, %lor.lhs.false3, %lor.lhs.false, %land.lhs.true
  %11 = load i32** %dir.addr, align 8
  %12 = load i32* %11, align 4
  %cmp9 = icmp eq i32 %12, 5
  br i1 %cmp9, label %if.then10, label %if.else

if.then10:                                        ; preds = %if.then
  %13 = load i32** %i.addr, align 8
  %14 = load i32* %13, align 4
  %inc = add i32 %14, 1
  store i32 %inc, i32* %13, align 4
  %15 = load i32** %dir.addr, align 8
  store i32 4, i32* %15, align 4
  br label %if.end30

if.else:                                          ; preds = %if.then
  %16 = load i32** %dir.addr, align 8
  %17 = load i32* %16, align 4
  %cmp11 = icmp eq i32 %17, 6
  br i1 %cmp11, label %if.then12, label %if.else14

if.then12:                                        ; preds = %if.else
  %18 = load i32** %j.addr, align 8
  %19 = load i32* %18, align 4
  %inc13 = add i32 %19, 1
  store i32 %inc13, i32* %18, align 4
  %20 = load i32** %dir.addr, align 8
  store i32 2, i32* %20, align 4
  br label %if.end29

if.else14:                                        ; preds = %if.else
  %21 = load i32** %dir.addr, align 8
  %22 = load i32* %21, align 4
  %cmp15 = icmp eq i32 %22, 7
  br i1 %cmp15, label %if.then16, label %if.else19

if.then16:                                        ; preds = %if.else14
  %23 = load i32** %i.addr, align 8
  %24 = load i32* %23, align 4
  %inc17 = add i32 %24, 1
  store i32 %inc17, i32* %23, align 4
  %25 = load i32** %j.addr, align 8
  %26 = load i32* %25, align 4
  %inc18 = add i32 %26, 1
  store i32 %inc18, i32* %25, align 4
  %27 = load i32** %dir.addr, align 8
  store i32 3, i32* %27, align 4
  br label %if.end28

if.else19:                                        ; preds = %if.else14
  %28 = load i32** %dir.addr, align 8
  %29 = load i32* %28, align 4
  %cmp20 = icmp eq i32 %29, 8
  br i1 %cmp20, label %if.then21, label %if.else23

if.then21:                                        ; preds = %if.else19
  %30 = load i32** %i.addr, align 8
  %31 = load i32* %30, align 4
  %inc22 = add i32 %31, 1
  store i32 %inc22, i32* %30, align 4
  %32 = load i32** %dir.addr, align 8
  store i32 1, i32* %32, align 4
  br label %if.end27

if.else23:                                        ; preds = %if.else19
  %33 = load i32** %dir.addr, align 8
  %34 = load i32* %33, align 4
  %cmp24 = icmp eq i32 %34, 4
  br i1 %cmp24, label %if.then25, label %if.end

if.then25:                                        ; preds = %if.else23
  %35 = load i32** %j.addr, align 8
  %36 = load i32* %35, align 4
  %inc26 = add i32 %36, 1
  store i32 %inc26, i32* %35, align 4
  br label %if.end

if.end:                                           ; preds = %if.then25, %if.else23
  br label %if.end27

if.end27:                                         ; preds = %if.end, %if.then21
  br label %if.end28

if.end28:                                         ; preds = %if.end27, %if.then16
  br label %if.end29

if.end29:                                         ; preds = %if.end28, %if.then12
  br label %if.end30

if.end30:                                         ; preds = %if.end29, %if.then10
  br label %if.end31

if.end31:                                         ; preds = %if.end30, %lor.lhs.false7, %entry
  ret void
}

; Function Attrs: nounwind uwtable
define linkonce_odr i32 @_ZNK5boxmg5bmg2d4core11GridStencil5indexEjjNS1_3DirE(%"class.boxmg::bmg2d::core::GridStencil"* %this, i32 %i, i32 %j, i32 %dir) #5 align 2 {
entry:
  %this.addr = alloca %"class.boxmg::bmg2d::core::GridStencil"*, align 8
  %i.addr = alloca i32, align 4
  %j.addr = alloca i32, align 4
  %dir.addr = alloca i32, align 4
  store %"class.boxmg::bmg2d::core::GridStencil"* %this, %"class.boxmg::bmg2d::core::GridStencil"** %this.addr, align 8
  store i32 %i, i32* %i.addr, align 4
  store i32 %j, i32* %j.addr, align 4
  store i32 %dir, i32* %dir.addr, align 4
  %this1 = load %"class.boxmg::bmg2d::core::GridStencil"** %this.addr
  %0 = load i32* %dir.addr, align 4
  %1 = bitcast %"class.boxmg::bmg2d::core::GridStencil"* %this1 to %"class.boxmg::bmg2d::core::Array"*
  %len_ = getelementptr inbounds %"class.boxmg::bmg2d::core::Array"* %1, i32 0, i32 2
  %call = call dereferenceable(4) i32* @_ZNKSt5arrayIjLm2EEixEm(%"struct.std::array"* %len_, i64 0) #2
  %2 = load i32* %call
  %mul = mul i32 %0, %2
  %3 = bitcast %"class.boxmg::bmg2d::core::GridStencil"* %this1 to %"class.boxmg::bmg2d::core::Array"*
  %len_2 = getelementptr inbounds %"class.boxmg::bmg2d::core::Array"* %3, i32 0, i32 2
  %call3 = call dereferenceable(4) i32* @_ZNKSt5arrayIjLm2EEixEm(%"struct.std::array"* %len_2, i64 1) #2
  %4 = load i32* %call3
  %mul4 = mul i32 %mul, %4
  %5 = load i32* %i.addr, align 4
  %6 = bitcast %"class.boxmg::bmg2d::core::GridStencil"* %this1 to %"class.boxmg::bmg2d::core::Array"*
  %stride_ = getelementptr inbounds %"class.boxmg::bmg2d::core::Array"* %6, i32 0, i32 3
  %call5 = call dereferenceable(4) i32* @_ZNKSt5arrayIiLm2EEixEm(%"struct.std::array.66"* %stride_, i64 0) #2
  %7 = load i32* %call5
  %mul6 = mul i32 %5, %7
  %add = add i32 %mul4, %mul6
  %8 = load i32* %j.addr, align 4
  %9 = bitcast %"class.boxmg::bmg2d::core::GridStencil"* %this1 to %"class.boxmg::bmg2d::core::Array"*
  %stride_7 = getelementptr inbounds %"class.boxmg::bmg2d::core::Array"* %9, i32 0, i32 3
  %call8 = call dereferenceable(4) i32* @_ZNKSt5arrayIiLm2EEixEm(%"struct.std::array.66"* %stride_7, i64 1) #2
  %10 = load i32* %call8
  %mul9 = mul i32 %8, %10
  %add10 = add i32 %add, %mul9
  ret i32 %add10
}

; Function Attrs: nounwind uwtable
define linkonce_odr double* @_ZNKSt6vectorId14AlignAllocatorIdLm16EEE4dataEv(%"class.std::vector.68"* %this) #5 align 2 {
entry:
  %this.addr = alloca %"class.std::vector.68"*, align 8
  store %"class.std::vector.68"* %this, %"class.std::vector.68"** %this.addr, align 8
  %this1 = load %"class.std::vector.68"** %this.addr
  %0 = bitcast %"class.std::vector.68"* %this1 to %"struct.std::_Vector_base.69"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base.69"* %0, i32 0, i32 0
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<double, AlignAllocator<double, 16> >::_Vector_impl"* %_M_impl, i32 0, i32 0
  %1 = load double** %_M_start, align 8
  %call = invoke double* @_ZNKSt6vectorId14AlignAllocatorIdLm16EEE11_M_data_ptrIdEEPT_S5_(%"class.std::vector.68"* %this1, double* %1)
          to label %invoke.cont unwind label %terminate.lpad

invoke.cont:                                      ; preds = %entry
  ret double* %call

terminate.lpad:                                   ; preds = %entry
  %2 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*)
          catch i8* null
  %3 = extractvalue { i8*, i32 } %2, 0
  call void @__clang_call_terminate(i8* %3) #10
  unreachable
}

; Function Attrs: nounwind uwtable
define linkonce_odr dereferenceable(4) i32* @_ZNKSt5arrayIjLm2EEixEm(%"struct.std::array"* %this, i64 %__n) #5 align 2 {
entry:
  %this.addr = alloca %"struct.std::array"*, align 8
  %__n.addr = alloca i64, align 8
  store %"struct.std::array"* %this, %"struct.std::array"** %this.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  %this1 = load %"struct.std::array"** %this.addr
  %_M_elems = getelementptr inbounds %"struct.std::array"* %this1, i32 0, i32 0
  %0 = load i64* %__n.addr, align 8
  %call = call dereferenceable(4) i32* @_ZNSt14__array_traitsIjLm2EE6_S_refERA2_Kjm([2 x i32]* dereferenceable(8) %_M_elems, i64 %0) #2
  ret i32* %call
}

; Function Attrs: nounwind uwtable
define linkonce_odr dereferenceable(4) i32* @_ZNKSt5arrayIiLm2EEixEm(%"struct.std::array.66"* %this, i64 %__n) #5 align 2 {
entry:
  %this.addr = alloca %"struct.std::array.66"*, align 8
  %__n.addr = alloca i64, align 8
  store %"struct.std::array.66"* %this, %"struct.std::array.66"** %this.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  %this1 = load %"struct.std::array.66"** %this.addr
  %_M_elems = getelementptr inbounds %"struct.std::array.66"* %this1, i32 0, i32 0
  %0 = load i64* %__n.addr, align 8
  %call = call dereferenceable(4) i32* @_ZNSt14__array_traitsIiLm2EE6_S_refERA2_Kim([2 x i32]* dereferenceable(8) %_M_elems, i64 %0) #2
  ret i32* %call
}

; Function Attrs: nounwind uwtable
define linkonce_odr dereferenceable(4) i32* @_ZNSt14__array_traitsIiLm2EE6_S_refERA2_Kim([2 x i32]* dereferenceable(8) %__t, i64 %__n) #5 align 2 {
entry:
  %__t.addr = alloca [2 x i32]*, align 8
  %__n.addr = alloca i64, align 8
  store [2 x i32]* %__t, [2 x i32]** %__t.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  %0 = load i64* %__n.addr, align 8
  %1 = load [2 x i32]** %__t.addr, align 8
  %arrayidx = getelementptr inbounds [2 x i32]* %1, i32 0, i64 %0
  ret i32* %arrayidx
}

; Function Attrs: nounwind uwtable
define linkonce_odr dereferenceable(4) i32* @_ZNSt14__array_traitsIjLm2EE6_S_refERA2_Kjm([2 x i32]* dereferenceable(8) %__t, i64 %__n) #5 align 2 {
entry:
  %__t.addr = alloca [2 x i32]*, align 8
  %__n.addr = alloca i64, align 8
  store [2 x i32]* %__t, [2 x i32]** %__t.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  %0 = load i64* %__n.addr, align 8
  %1 = load [2 x i32]** %__t.addr, align 8
  %arrayidx = getelementptr inbounds [2 x i32]* %1, i32 0, i64 %0
  ret i32* %arrayidx
}

define internal void @_GLOBAL__sub_I_residual.cc() section ".text.startup" {
entry:
  call void @__cxx_global_var_init()
  call void @__cxx_global_var_init1()
  call void @__cxx_global_var_init2()
  call void @__cxx_global_var_init3()
  call void @__cxx_global_var_init4()
  call void @__cxx_global_var_init5()
  call void @__cxx_global_var_init6()
  call void @__cxx_global_var_init7()
  call void @__cxx_global_var_init8()
  call void @__cxx_global_var_init9()
  call void @__cxx_global_var_init10()
  call void @__cxx_global_var_init11()
  call void @__cxx_global_var_init12()
  call void @__cxx_global_var_init13()
  call void @__cxx_global_var_init14()
  call void @__cxx_global_var_init15()
  call void @__cxx_global_var_init16()
  call void @__cxx_global_var_init17()
  call void @__cxx_global_var_init18()
  call void @__cxx_global_var_init19()
  call void @__cxx_global_var_init20()
  call void @__cxx_global_var_init21()
  call void @__cxx_global_var_init22()
  call void @__cxx_global_var_init23()
  call void @__cxx_global_var_init24()
  call void @__cxx_global_var_init25()
  call void @__cxx_global_var_init26()
  call void @__cxx_global_var_init27()
  call void @__cxx_global_var_init28()
  call void @__cxx_global_var_init29()
  call void @__cxx_global_var_init30()
  call void @__cxx_global_var_init31()
  call void @__cxx_global_var_init32()
  call void @__cxx_global_var_init33()
  call void @__cxx_global_var_init34()
  call void @__cxx_global_var_init35()
  call void @__cxx_global_var_init36()
  call void @__cxx_global_var_init37()
  call void @__cxx_global_var_init38()
  call void @__cxx_global_var_init39()
  call void @__cxx_global_var_init40()
  call void @__cxx_global_var_init41()
  call void @__cxx_global_var_init42()
  call void @__cxx_global_var_init43()
  call void @__cxx_global_var_init44()
  ret void
}

attributes #0 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }
attributes #3 = { inlinehint nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nobuiltin nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { inlinehint uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { noinline noreturn nounwind }
attributes #9 = { builtin nounwind }
attributes #10 = { noreturn nounwind }
