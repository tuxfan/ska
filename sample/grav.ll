; ModuleID = 'grav.c'
target datalayout = "e-p:64:64:64-S128-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f16:16:16-f32:32:32-f64:64:64-f128:128:128-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

module asm "\09.ident\09\22GCC: (GNU) 4.7.0 LLVM: 3.1\22"

define void @do_grav_sse4(<4 x float>* %f, <4 x float>* %fend, float* nocapture %pos0, float* nocapture %mass0, float* nocapture %acc, float* nocapture %phi0, float* nocapture %eps2p, i32* nocapture %ncut) nounwind uwtable {
entry:
  %0 = load float* %pos0, align 4, !tbaa !0
  %1 = insertelement <4 x float> undef, float %0, i32 0
  %2 = insertelement <4 x float> %1, float %0, i32 1
  %3 = insertelement <4 x float> %2, float %0, i32 2
  %4 = insertelement <4 x float> %3, float %0, i32 3
  %5 = getelementptr inbounds float* %pos0, i64 1
  %6 = load float* %5, align 4, !tbaa !0
  %7 = insertelement <4 x float> undef, float %6, i32 0
  %8 = insertelement <4 x float> %7, float %6, i32 1
  %9 = insertelement <4 x float> %8, float %6, i32 2
  %10 = insertelement <4 x float> %9, float %6, i32 3
  %11 = getelementptr inbounds float* %pos0, i64 2
  %12 = load float* %11, align 4, !tbaa !0
  %13 = insertelement <4 x float> undef, float %12, i32 0
  %14 = insertelement <4 x float> %13, float %12, i32 1
  %15 = insertelement <4 x float> %14, float %12, i32 2
  %16 = insertelement <4 x float> %15, float %12, i32 3
  %17 = load float* %eps2p, align 4, !tbaa !0
  %18 = insertelement <4 x float> undef, float %17, i32 0
  %19 = insertelement <4 x float> %18, float %17, i32 1
  %20 = insertelement <4 x float> %19, float %17, i32 2
  %21 = insertelement <4 x float> %20, float %17, i32 3
  %22 = icmp ult <4 x float>* %f, %fend
  br i1 %22, label %"3", label %"5"

"3":                                              ; preds = %entry, %"3"
  %23 = phi <4 x float> [ %53, %"3" ], [ zeroinitializer, %entry ]
  %24 = phi <4 x float> [ %60, %"3" ], [ zeroinitializer, %entry ]
  %25 = phi <4 x float> [ %58, %"3" ], [ zeroinitializer, %entry ]
  %26 = phi <4 x float> [ %56, %"3" ], [ zeroinitializer, %entry ]
  %27 = phi <4 x float>* [ %52, %"3" ], [ %f, %entry ]
  %28 = getelementptr inbounds <4 x float>* %27, i64 1
  %29 = load <4 x float>* %28, align 16, !tbaa !0
  %30 = fsub <4 x float> %4, %29
  %31 = getelementptr inbounds <4 x float>* %27, i64 2
  %32 = load <4 x float>* %31, align 16, !tbaa !0
  %33 = fsub <4 x float> %10, %32
  %34 = getelementptr inbounds <4 x float>* %27, i64 3
  %35 = load <4 x float>* %34, align 16, !tbaa !0
  %36 = fsub <4 x float> %16, %35
  %37 = fmul <4 x float> %30, %30
  %38 = fmul <4 x float> %33, %33
  %39 = fadd <4 x float> %37, %38
  %40 = fmul <4 x float> %36, %36
  %41 = fadd <4 x float> %39, %40
  %42 = fadd <4 x float> %41, %21
  %43 = tail call <4 x float> @llvm.x86.sse.rsqrt.ps(<4 x float> %42) nounwind readnone
  %44 = fmul <4 x float> %42, %43
  %45 = fmul <4 x float> %43, %44
  %46 = fadd <4 x float> %45, <float -3.000000e+00, float -3.000000e+00, float -3.000000e+00, float -3.000000e+00>
  %47 = fmul <4 x float> %46, %43
  %48 = fmul <4 x float> %47, <float 5.000000e-01, float 5.000000e-01, float 5.000000e-01, float 5.000000e-01>
  %49 = fmul <4 x float> %48, %48
  %50 = load <4 x float>* %27, align 16, !tbaa !0
  %51 = fmul <4 x float> %50, %48
  %52 = getelementptr inbounds <4 x float>* %27, i64 4
  %53 = fadd <4 x float> %23, %51
  %54 = fmul <4 x float> %49, %51
  %55 = fmul <4 x float> %30, %54
  %56 = fadd <4 x float> %55, %26
  %57 = fmul <4 x float> %33, %54
  %58 = fadd <4 x float> %57, %25
  %59 = fmul <4 x float> %36, %54
  %60 = fadd <4 x float> %59, %24
  %61 = icmp ult <4 x float>* %52, %fend
  br i1 %61, label %"3", label %"5"

"5":                                              ; preds = %"3", %entry
  %.lcssa3 = phi <4 x float> [ zeroinitializer, %entry ], [ %53, %"3" ]
  %.lcssa2 = phi <4 x float> [ zeroinitializer, %entry ], [ %60, %"3" ]
  %.lcssa1 = phi <4 x float> [ zeroinitializer, %entry ], [ %58, %"3" ]
  %.lcssa = phi <4 x float> [ zeroinitializer, %entry ], [ %56, %"3" ]
  %62 = tail call <4 x float> @llvm.x86.sse3.hadd.ps(<4 x float> %.lcssa3, <4 x float> %.lcssa3) nounwind readnone
  %63 = tail call <4 x float> @llvm.x86.sse3.hadd.ps(<4 x float> %.lcssa, <4 x float> %.lcssa) nounwind readnone
  %64 = tail call <4 x float> @llvm.x86.sse3.hadd.ps(<4 x float> %.lcssa1, <4 x float> %.lcssa1) nounwind readnone
  %65 = tail call <4 x float> @llvm.x86.sse3.hadd.ps(<4 x float> %.lcssa2, <4 x float> %.lcssa2) nounwind readnone
  %66 = load float* %phi0, align 4, !tbaa !0
  %67 = tail call <4 x float> @llvm.x86.sse3.hadd.ps(<4 x float> %62, <4 x float> %62) nounwind readnone
  %68 = extractelement <4 x float> %67, i32 0
  %69 = fadd float %66, %68
  store float %69, float* %phi0, align 4, !tbaa !0
  %70 = load float* %acc, align 4, !tbaa !0
  %71 = tail call <4 x float> @llvm.x86.sse3.hadd.ps(<4 x float> %63, <4 x float> %63) nounwind readnone
  %72 = extractelement <4 x float> %71, i32 0
  %73 = fadd float %70, %72
  store float %73, float* %acc, align 4, !tbaa !0
  %74 = getelementptr inbounds float* %acc, i64 1
  %75 = load float* %74, align 4, !tbaa !0
  %76 = tail call <4 x float> @llvm.x86.sse3.hadd.ps(<4 x float> %64, <4 x float> %64) nounwind readnone
  %77 = extractelement <4 x float> %76, i32 0
  %78 = fadd float %75, %77
  store float %78, float* %74, align 4, !tbaa !0
  %79 = getelementptr inbounds float* %acc, i64 2
  %80 = load float* %79, align 4, !tbaa !0
  %81 = tail call <4 x float> @llvm.x86.sse3.hadd.ps(<4 x float> %65, <4 x float> %65) nounwind readnone
  %82 = extractelement <4 x float> %81, i32 0
  %83 = fadd float %80, %82
  store float %83, float* %79, align 4, !tbaa !0
  ret void
}

declare <4 x float> @llvm.x86.sse.rsqrt.ps(<4 x float>) nounwind readnone

declare <4 x float> @llvm.x86.sse3.hadd.ps(<4 x float>, <4 x float>) nounwind readnone

!0 = metadata !{metadata !"alias set 7: float", metadata !1}
!1 = metadata !{metadata !1}
