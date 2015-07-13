; ModuleID = 'main.c'
target datalayout = "e-p:64:64:64-S128-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f16:16:16-f32:32:32-f64:64:64-f128:128:128-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

module asm "\09.ident\09\22GCC: (GNU) 4.7.0 LLVM: 3.1\22"

define i32 @main(i32 %argc, i8** nocapture %argv) nounwind uwtable readnone {
entry:
  %a1 = alloca [32 x float], align 4
  %b2 = alloca [32 x float], align 4
  %c3 = alloca [32 x float], align 4
  br label %"3"

"3":                                              ; preds = %"3", %entry
  %indvars.iv = phi i64 [ 0, %entry ], [ %indvars.iv.next, %"3" ]
  %0 = getelementptr [32 x float]* %a1, i64 0, i64 %indvars.iv
  %1 = load float* %0, align 4
  %2 = getelementptr [32 x float]* %b2, i64 0, i64 %indvars.iv
  %3 = load float* %2, align 4
  %4 = getelementptr [32 x float]* %c3, i64 0, i64 %indvars.iv
  %5 = load float* %4, align 4
  %6 = fmul float %3, %5
  %7 = fadd float %1, %6
  store float %7, float* %4, align 4
  %indvars.iv.next = add i64 %indvars.iv, 1
  %lftr.wideiv = trunc i64 %indvars.iv.next to i32
  %exitcond = icmp eq i32 %lftr.wideiv, 32
  br i1 %exitcond, label %"5", label %"3"

"5":                                              ; preds = %"3"
  ret i32 0
}
