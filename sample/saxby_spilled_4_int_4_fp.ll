  %0 = alloca [32 x float]*
  %a1 = alloca [32 x float], align 4
  %1 = alloca [32 x float]*
  store [32 x float]* %a1, [32 x float]** %1
  %b2 = alloca [32 x float], align 4
  %c3 = alloca [32 x float], align 4
  %2 = alloca [32 x float]*
  store [32 x float]* %c3, [32 x float]** %2
  br label %"3"
  %indvars.iv = phi i64 [ 0, %entry ], [ %indvars.iv.next, %"3" ]
  %3 = alloca i64
  store i64 %indvars.iv, i64* %3
  %LOAD1 = load [32 x float]** %0
  %LOAD13 = load [32 x float]** %0
  %4 = getelementptr [32 x float]* %LOAD13, i64 0, [32 x float]* %LOAD1
  store [32 x float]* %LOAD13, [32 x float]** %0
  %5 = alloca float*
  store float* %4, float** %5
  store [32 x float]* %LOAD1, [32 x float]** %0
  %LOAD5 = load [32 x float]** %0
  %6 = load [32 x float]* %LOAD5, align 4
  %7 = alloca float
  store float %6, float* %7
  store [32 x float]* %LOAD5, [32 x float]** %0
  %LOAD2 = load [32 x float]** %0
  %8 = getelementptr [32 x float]* %b2, i64 0, [32 x float]* %LOAD2
  %9 = alloca float*
  store float* %8, float** %9
  store [32 x float]* %LOAD2, [32 x float]** %0
  %LOAD7 = load [32 x float]** %0
  %10 = load [32 x float]* %LOAD7, align 4
  %11 = alloca float
  store float %10, float* %11
  store [32 x float]* %LOAD7, [32 x float]** %0
  %LOAD = load [32 x float]** %0
  %LOAD3 = load [32 x float]** %0
  %12 = getelementptr [32 x float]* %LOAD, i64 0, [32 x float]* %LOAD3
  store [32 x float]* %LOAD3, [32 x float]** %0
  store [32 x float]* %LOAD, [32 x float]** %0
  %13 = load float* %12, align 4
  %14 = alloca float
  store float %13, float* %14
  %LOAD8 = load [32 x float]** %0
  %LOAD9 = load [32 x float]** %0
  %15 = fmul [32 x float]* %LOAD8, %LOAD9
  %16 = alloca float
  store float %15, float* %16
  store [32 x float]* %LOAD9, [32 x float]** %0
  store [32 x float]* %LOAD8, [32 x float]** %0
  %LOAD6 = load [32 x float]** %0
  %LOAD10 = load [32 x float]** %0
  %17 = fadd [32 x float]* %LOAD6, %LOAD10
  %18 = alloca float
  store float %17, float* %18
  store [32 x float]* %LOAD10, [32 x float]** %0
  store [32 x float]* %LOAD6, [32 x float]** %0
  %LOAD11 = load [32 x float]** %0
  store [32 x float]* %LOAD11, float* %12, align 4
  store [32 x float]* %LOAD11, [32 x float]** %0
  %LOAD4 = load [32 x float]** %0
  %indvars.iv.next = add [32 x float]* %LOAD4, i64 1
  store [32 x float]* %LOAD4, [32 x float]** %0
  %lftr.wideiv = trunc i64 %indvars.iv.next to i32
  %19 = alloca i32
  store i32 %lftr.wideiv, i32* %19
  %LOAD12 = load [32 x float]** %0
  %exitcond = icmp eq [32 x float]* %LOAD12, i32 32
  store [32 x float]* %LOAD12, [32 x float]** %0
  br i1 %exitcond, label %"5", label %"3"
  ret i32 0

