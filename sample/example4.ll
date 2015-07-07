; ModuleID = 'example4.cc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @_Z5main1bb(i1 zeroext %r, i1 zeroext %y) #0 {
entry:
  %r.addr = alloca i8, align 1
  %y.addr = alloca i8, align 1
  %l = alloca i8, align 1
  %frombool = zext i1 %r to i8
  store i8 %frombool, i8* %r.addr, align 1
  %frombool1 = zext i1 %y to i8
  store i8 %frombool1, i8* %y.addr, align 1
  %0 = load i8* %y.addr, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %lor.end, label %lor.rhs

lor.rhs:                                          ; preds = %entry
  %1 = load i8* %r.addr, align 1
  %tobool2 = trunc i8 %1 to i1
  br label %lor.end

lor.end:                                          ; preds = %lor.rhs, %entry
  %2 = phi i1 [ true, %entry ], [ %tobool2, %lor.rhs ]
  %frombool3 = zext i1 %2 to i8
  store i8 %frombool3, i8* %l, align 1
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
