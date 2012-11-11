; ModuleID = 'saxpy.c'
target datalayout = "e-p:64:64:64-S128-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f16:16:16-f32:32:32-f64:64:64-f128:128:128-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-apple-darwin11"

define void @saxpy(i32 %n, float %alpha, float* nocapture %x, float* nocapture %y) nounwind uwtable {
entry:
  %0 = icmp sgt i32 %n, 0
  br i1 %0, label %"3.lr.ph", label %return

"3.lr.ph":                                        ; preds = %entry
  %1 = bitcast float* %y to i8*
  %2 = bitcast float* %x to i8*
  br label %"3"

"3":                                              ; preds = %"3", %"3.lr.ph"
  %indvars.iv = phi i64 [ 0, %"3.lr.ph" ], [ %indvars.iv.next, %"3" ]
  %3 = shl nsw i64 %indvars.iv, 2
  %4 = getelementptr inbounds i8* %1, i64 %3
  %5 = getelementptr inbounds i8* %2, i64 %3
  %6 = bitcast i8* %5 to float*
  %7 = load float* %6, align 4, !tbaa !0
  %8 = fmul float %7, %alpha
  %9 = bitcast i8* %4 to float*
  %10 = load float* %9, align 4, !tbaa !0
  %11 = fadd float %8, %10
  store float %11, float* %9, align 4, !tbaa !0
  %indvars.iv.next = add i64 %indvars.iv, 1
  %lftr.wideiv = trunc i64 %indvars.iv.next to i32
  %exitcond = icmp eq i32 %lftr.wideiv, %n
  br i1 %exitcond, label %return, label %"3"

return:                                           ; preds = %"3", %entry
  ret void
}

!0 = metadata !{metadata !"alias set 7: float", metadata !1}
!1 = metadata !{metadata !1}
