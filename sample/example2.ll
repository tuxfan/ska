; ModuleID = 'example2.cc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @_Z3addiii(i32 %x, i32 %y, i32 %z) #0 {
entry:
  %x.addr = alloca i32, align 4
  %y.addr = alloca i32, align 4
  %z.addr = alloca i32, align 4
  store i32 %x, i32* %x.addr, align 4
  store i32 %y, i32* %y.addr, align 4
  store i32 %z, i32* %z.addr, align 4
  %0 = load i32* %z.addr, align 4
  %1 = load i32* %x.addr, align 4
  %add = add nsw i32 %0, %1
  %2 = load i32* %y.addr, align 4
  %add1 = add nsw i32 %add, %2
  ret i32 %add1
}

; Function Attrs: nounwind uwtable
define i32 @_Z7getDataii(i32 %x, i32 %y) #0 {
entry:
  %x.addr = alloca i32, align 4
  %y.addr = alloca i32, align 4
  %z = alloca i32, align 4
  store i32 %x, i32* %x.addr, align 4
  store i32 %y, i32* %y.addr, align 4
  %0 = load i32* %x.addr, align 4
  %1 = load i32* %y.addr, align 4
  %add = add nsw i32 %0, %1
  store i32 %add, i32* %z, align 4
  %2 = load i32* %z, align 4
  %3 = load i32* %x.addr, align 4
  %4 = load i32* %y.addr, align 4
  %5 = load i32* %z, align 4
  %call = call i32 @_Z3addiii(i32 %3, i32 %4, i32 %5)
  %add1 = add nsw i32 %2, %call
  store i32 %add1, i32* %z, align 4
  %6 = load i32* %z, align 4
  ret i32 %6
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

