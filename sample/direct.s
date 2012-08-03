; ModuleID = 'grav.c'
target datalayout = "e-p:64:64:64-S128-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f16:16:16-f32:32:32-f64:64:64-f128:128:128-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64-unknown-linux-gnu"

module asm "\09.ident\09\22GCC: (GNU) 4.7.0 LLVM: 3.1\22"

%struct.Particle = type { i32, i32, float, float, [3 x float], [3 x float], float }

define void @diaGrav(%struct.Particle* nocapture %p, i32 %bs, i32 %iSoftType) nounwind uwtable {
entry:
  %0 = bitcast %struct.Particle* %p to i8*
  %1 = icmp eq i32 %iSoftType, 1
  br label %"17"

"4":                                              ; preds = %"4.lr.ph", %"14"
  %indvars.iv36 = phi i64 [ %indvars.iv.next37, %"14" ], [ %indvars.iv38, %"4.lr.ph" ]
  %2 = phi double [ %119, %"14" ], [ 0.000000e+00, %"4.lr.ph" ]
  %3 = phi double [ %128, %"14" ], [ 0.000000e+00, %"4.lr.ph" ]
  %4 = phi double [ %125, %"14" ], [ 0.000000e+00, %"4.lr.ph" ]
  %5 = phi double [ %122, %"14" ], [ 0.000000e+00, %"4.lr.ph" ]
  %6 = mul i64 %indvars.iv36, 44
  %.sum4 = add i64 %6, 16
  %7 = getelementptr inbounds i8* %0, i64 %.sum4
  %8 = bitcast i8* %7 to float*
  %9 = load float* %8, align 4, !tbaa !0
  %10 = load float* %198, align 4, !tbaa !0
  %11 = fsub float %9, %10
  %12 = fpext float %11 to double
  %.sum6 = add i64 %6, 20
  %13 = getelementptr inbounds i8* %0, i64 %.sum6
  %14 = bitcast i8* %13 to float*
  %15 = load float* %14, align 4, !tbaa !0
  %16 = load float* %200, align 4, !tbaa !0
  %17 = fsub float %15, %16
  %18 = fpext float %17 to double
  %.sum8 = add i64 %6, 24
  %19 = getelementptr inbounds i8* %0, i64 %.sum8
  %20 = bitcast i8* %19 to float*
  %21 = load float* %20, align 4, !tbaa !0
  %22 = load float* %202, align 4, !tbaa !0
  %23 = fsub float %21, %22
  %24 = fpext float %23 to double
  %25 = fmul double %12, %12
  %26 = fmul double %18, %18
  %27 = fadd double %25, %26
  %28 = fmul double %24, %24
  %29 = fadd double %27, %28
  %30 = load float* %204, align 4, !tbaa !0
  %.sum11 = add i64 %6, 12
  %31 = getelementptr inbounds i8* %0, i64 %.sum11
  %32 = bitcast i8* %31 to float*
  %33 = load float* %32, align 4, !tbaa !0
  %34 = fadd float %30, %33
  %35 = fpext float %34 to double
  %cond = icmp eq i32 %iSoftType, 2
  br i1 %cond, label %"5", label %"9"

"5":                                              ; preds = %"4"
  %36 = fmul double %35, 2.500000e-01
  %37 = fmul double %36, %35
  %38 = fadd double %37, %29
  %39 = tail call double @sqrt(double %38) nounwind
  %40 = fdiv double 1.000000e+00, %39
  %41 = fmul double %40, %40
  %42 = fmul double %41, %40
  br label %"14"

"9":                                              ; preds = %"4"
  %43 = tail call double @sqrt(double %29) nounwind
  %44 = fcmp olt double %43, %35
  br i1 %44, label %"10", label %"13"

"10":                                             ; preds = %"9"
  %45 = fdiv double 2.000000e+00, %35
  %46 = fmul double %43, %45
  %47 = fcmp olt double %46, 1.000000e+00
  br i1 %47, label %"11", label %"12"

"11":                                             ; preds = %"10"
  %48 = fmul double %46, 0x3FE5555555555555
  %49 = fmul double %48, %46
  %50 = fsub double 1.400000e+00, %49
  %51 = fmul double %46, 3.000000e-01
  %52 = fmul double %51, %46
  %53 = fmul double %52, %46
  %54 = fmul double %53, %46
  %55 = fadd double %50, %54
  %56 = fmul double %46, 1.000000e-01
  %57 = fmul double %56, %46
  %58 = fmul double %57, %46
  %59 = fmul double %58, %46
  %60 = fmul double %59, %46
  %61 = fsub double %55, %60
  %62 = fmul double %61, %45
  %63 = fmul double %45, %45
  %64 = fmul double %63, %45
  %65 = fmul double %46, 1.200000e+00
  %66 = fmul double %65, %46
  %67 = fsub double 0x3FF5555555555555, %66
  %68 = fmul double %46, 5.000000e-01
  %69 = fmul double %68, %46
  %70 = fmul double %69, %46
  %71 = fadd double %67, %70
  %72 = fmul double %64, %71
  br label %"14"

"12":                                             ; preds = %"10"
  %73 = fdiv double 1.000000e+00, %43
  %74 = fmul double %73, 0xBFB1111111111111
  %75 = fmul double %46, 0x3FF5555555555555
  %76 = fmul double %75, %46
  %77 = fsub double 1.600000e+00, %76
  %78 = fmul double %46, %46
  %79 = fmul double %78, %46
  %80 = fadd double %77, %79
  %81 = fmul double %46, 3.000000e-01
  %82 = fmul double %81, %46
  %83 = fmul double %82, %46
  %84 = fmul double %83, %46
  %85 = fsub double %80, %84
  %86 = fmul double %46, 0x3FA1111111111111
  %87 = fmul double %86, %46
  %88 = fmul double %87, %46
  %89 = fmul double %88, %46
  %90 = fmul double %89, %46
  %91 = fadd double %85, %90
  %92 = fmul double %91, %45
  %93 = fadd double %74, %92
  %94 = fmul double %74, %73
  %95 = fmul double %94, %73
  %96 = fmul double %45, %45
  %97 = fmul double %96, %45
  %98 = fmul double %46, 3.000000e+00
  %99 = fsub double 0x4005555555555555, %98
  %100 = fmul double %46, 1.200000e+00
  %101 = fmul double %100, %46
  %102 = fadd double %99, %101
  %103 = fmul double %46, 0x3FC5555555555555
  %104 = fmul double %103, %46
  %105 = fmul double %104, %46
  %106 = fsub double %102, %105
  %107 = fmul double %97, %106
  %108 = fadd double %95, %107
  br label %"14"

"13":                                             ; preds = %"9"
  %109 = fdiv double 1.000000e+00, %43
  %110 = fmul double %109, %109
  %111 = fmul double %110, %109
  br label %"14"

"14":                                             ; preds = %"13", %"12", %"11", %"5"
  %112 = phi double [ %109, %"13" ], [ %93, %"12" ], [ %62, %"11" ], [ %40, %"5" ]
  %113 = phi double [ %111, %"13" ], [ %108, %"12" ], [ %72, %"11" ], [ %42, %"5" ]
  %.sum12 = add i64 %6, 8
  %114 = getelementptr inbounds i8* %0, i64 %.sum12
  %115 = bitcast i8* %114 to float*
  %116 = load float* %115, align 4, !tbaa !0
  %117 = fpext float %116 to double
  %118 = fmul double %117, %112
  %119 = fsub double %2, %118
  %120 = fmul double %117, %12
  %121 = fmul double %120, %113
  %122 = fadd double %121, %5
  %123 = fmul double %117, %18
  %124 = fmul double %123, %113
  %125 = fadd double %124, %4
  %126 = fmul double %117, %24
  %127 = fmul double %126, %113
  %128 = fadd double %127, %3
  %.sum13 = add i64 %6, 40
  %129 = getelementptr inbounds i8* %0, i64 %.sum13
  %130 = bitcast i8* %129 to float*
  %131 = load float* %130, align 4, !tbaa !0
  %132 = fpext float %131 to double
  %133 = load float* %206, align 4, !tbaa !0
  %134 = fpext float %133 to double
  %135 = fmul double %134, %112
  %136 = fsub double %132, %135
  %137 = fptrunc double %136 to float
  store float %137, float* %130, align 4, !tbaa !0
  %.sum15 = add i64 %6, 28
  %138 = getelementptr inbounds i8* %0, i64 %.sum15
  %139 = bitcast i8* %138 to float*
  %140 = load float* %139, align 4, !tbaa !0
  %141 = fpext float %140 to double
  %142 = load float* %206, align 4, !tbaa !0
  %143 = fpext float %142 to double
  %144 = fmul double %143, %12
  %145 = fmul double %144, %113
  %146 = fsub double %141, %145
  %147 = fptrunc double %146 to float
  store float %147, float* %139, align 4, !tbaa !0
  %.sum16 = add i64 %6, 32
  %148 = getelementptr inbounds i8* %0, i64 %.sum16
  %149 = bitcast i8* %148 to float*
  %150 = load float* %149, align 4, !tbaa !0
  %151 = fpext float %150 to double
  %152 = load float* %206, align 4, !tbaa !0
  %153 = fpext float %152 to double
  %154 = fmul double %153, %18
  %155 = fmul double %154, %113
  %156 = fsub double %151, %155
  %157 = fptrunc double %156 to float
  store float %157, float* %149, align 4, !tbaa !0
  %.sum17 = add i64 %6, 36
  %158 = getelementptr inbounds i8* %0, i64 %.sum17
  %159 = bitcast i8* %158 to float*
  %160 = load float* %159, align 4, !tbaa !0
  %161 = fpext float %160 to double
  %162 = load float* %206, align 4, !tbaa !0
  %163 = fpext float %162 to double
  %164 = fmul double %163, %24
  %165 = fmul double %164, %113
  %166 = fsub double %161, %165
  %167 = fptrunc double %166 to float
  store float %167, float* %159, align 4, !tbaa !0
  %indvars.iv.next37 = add i64 %indvars.iv36, 1
  %168 = trunc i64 %indvars.iv.next37 to i32
  %169 = icmp slt i32 %168, %bs
  br i1 %169, label %"4", label %"16"

"16":                                             ; preds = %"14.us", %"14"
  %.lcssa20 = phi double [ %119, %"14" ], [ %214, %"14.us" ]
  %.lcssa19 = phi double [ %128, %"14" ], [ %223, %"14.us" ]
  %.lcssa18 = phi double [ %125, %"14" ], [ %220, %"14.us" ]
  %.lcssa = phi double [ %122, %"14" ], [ %217, %"14.us" ]
  %.sum = add i64 %196, -16
  %170 = getelementptr inbounds i8* %0, i64 %.sum
  %171 = bitcast i8* %170 to float*
  %172 = load float* %171, align 4, !tbaa !0
  %173 = fpext float %172 to double
  %174 = fadd double %173, %.lcssa
  %175 = fptrunc double %174 to float
  store float %175, float* %171, align 4, !tbaa !0
  %.sum1 = add i64 %196, -12
  %176 = getelementptr inbounds i8* %0, i64 %.sum1
  %177 = bitcast i8* %176 to float*
  %178 = load float* %177, align 4, !tbaa !0
  %179 = fpext float %178 to double
  %180 = fadd double %179, %.lcssa18
  %181 = fptrunc double %180 to float
  store float %181, float* %177, align 4, !tbaa !0
  %.sum2 = add i64 %196, -8
  %182 = getelementptr inbounds i8* %0, i64 %.sum2
  %183 = bitcast i8* %182 to float*
  %184 = load float* %183, align 4, !tbaa !0
  %185 = fpext float %184 to double
  %186 = fadd double %185, %.lcssa19
  %187 = fptrunc double %186 to float
  store float %187, float* %183, align 4, !tbaa !0
  %.sum3 = add i64 %196, -4
  %188 = getelementptr inbounds i8* %0, i64 %.sum3
  %189 = bitcast i8* %188 to float*
  %190 = load float* %189, align 4, !tbaa !0
  %191 = fpext float %190 to double
  %192 = fadd double %191, %.lcssa20
  %193 = fptrunc double %192 to float
  store float %193, float* %189, align 4, !tbaa !0
  %indvars.iv.next39 = add i64 %indvars.iv38, 1
  br label %"17"

"17":                                             ; preds = %"16", %entry
  %indvars.iv38 = phi i64 [ %indvars.iv.next39, %"16" ], [ 1, %entry ]
  %194 = trunc i64 %indvars.iv38 to i32
  %195 = icmp slt i32 %194, %bs
  br i1 %195, label %"4.lr.ph", label %return

"4.lr.ph":                                        ; preds = %"17"
  %196 = mul i64 %indvars.iv38, 44
  %.sum5 = add i64 %196, -28
  %197 = getelementptr inbounds i8* %0, i64 %.sum5
  %198 = bitcast i8* %197 to float*
  %.sum7 = add i64 %196, -24
  %199 = getelementptr inbounds i8* %0, i64 %.sum7
  %200 = bitcast i8* %199 to float*
  %.sum9 = add i64 %196, -20
  %201 = getelementptr inbounds i8* %0, i64 %.sum9
  %202 = bitcast i8* %201 to float*
  %.sum10 = add i64 %196, -32
  %203 = getelementptr inbounds i8* %0, i64 %.sum10
  %204 = bitcast i8* %203 to float*
  %.sum14 = add i64 %196, -36
  %205 = getelementptr inbounds i8* %0, i64 %.sum14
  %206 = bitcast i8* %205 to float*
  br i1 %1, label %"6.us", label %"4"

"14.us":                                          ; preds = %"8.us", %"7.us"
  %207 = phi double [ %307, %"8.us" ], [ %268, %"7.us" ]
  %208 = phi double [ %309, %"8.us" ], [ %264, %"7.us" ]
  %.sum12.us = add i64 %273, 8
  %209 = getelementptr inbounds i8* %0, i64 %.sum12.us
  %210 = bitcast i8* %209 to float*
  %211 = load float* %210, align 4, !tbaa !0
  %212 = fpext float %211 to double
  %213 = fmul double %212, %207
  %214 = fsub double %269, %213
  %215 = fmul double %212, %279
  %216 = fmul double %215, %208
  %217 = fadd double %216, %272
  %218 = fmul double %212, %285
  %219 = fmul double %218, %208
  %220 = fadd double %219, %271
  %221 = fmul double %212, %291
  %222 = fmul double %221, %208
  %223 = fadd double %222, %270
  %.sum13.us = add i64 %273, 40
  %224 = getelementptr inbounds i8* %0, i64 %.sum13.us
  %225 = bitcast i8* %224 to float*
  %226 = load float* %225, align 4, !tbaa !0
  %227 = fpext float %226 to double
  %228 = load float* %206, align 4, !tbaa !0
  %229 = fpext float %228 to double
  %230 = fmul double %229, %207
  %231 = fsub double %227, %230
  %232 = fptrunc double %231 to float
  store float %232, float* %225, align 4, !tbaa !0
  %.sum15.us = add i64 %273, 28
  %233 = getelementptr inbounds i8* %0, i64 %.sum15.us
  %234 = bitcast i8* %233 to float*
  %235 = load float* %234, align 4, !tbaa !0
  %236 = fpext float %235 to double
  %237 = load float* %206, align 4, !tbaa !0
  %238 = fpext float %237 to double
  %239 = fmul double %238, %279
  %240 = fmul double %239, %208
  %241 = fsub double %236, %240
  %242 = fptrunc double %241 to float
  store float %242, float* %234, align 4, !tbaa !0
  %.sum16.us = add i64 %273, 32
  %243 = getelementptr inbounds i8* %0, i64 %.sum16.us
  %244 = bitcast i8* %243 to float*
  %245 = load float* %244, align 4, !tbaa !0
  %246 = fpext float %245 to double
  %247 = load float* %206, align 4, !tbaa !0
  %248 = fpext float %247 to double
  %249 = fmul double %248, %285
  %250 = fmul double %249, %208
  %251 = fsub double %246, %250
  %252 = fptrunc double %251 to float
  store float %252, float* %244, align 4, !tbaa !0
  %.sum17.us = add i64 %273, 36
  %253 = getelementptr inbounds i8* %0, i64 %.sum17.us
  %254 = bitcast i8* %253 to float*
  %255 = load float* %254, align 4, !tbaa !0
  %256 = fpext float %255 to double
  %257 = load float* %206, align 4, !tbaa !0
  %258 = fpext float %257 to double
  %259 = fmul double %258, %291
  %260 = fmul double %259, %208
  %261 = fsub double %256, %260
  %262 = fptrunc double %261 to float
  store float %262, float* %254, align 4, !tbaa !0
  %indvars.iv.next29 = add i64 %indvars.iv28, 1
  %lftr.wideiv = trunc i64 %indvars.iv.next29 to i32
  %exitcond = icmp eq i32 %lftr.wideiv, %bs
  br i1 %exitcond, label %"16", label %"6.us"

"7.us":                                           ; preds = %"6.us"
  %263 = fmul double %302, %304
  %264 = fdiv double 2.000000e+00, %263
  %265 = fmul double %264, 5.000000e-01
  %266 = fmul double %304, 3.000000e+00
  %267 = fsub double %266, %296
  %268 = fmul double %265, %267
  br label %"14.us"

"6.us":                                           ; preds = %"14.us", %"4.lr.ph"
  %indvars.iv28 = phi i64 [ %indvars.iv.next29, %"14.us" ], [ %indvars.iv38, %"4.lr.ph" ]
  %269 = phi double [ %214, %"14.us" ], [ 0.000000e+00, %"4.lr.ph" ]
  %270 = phi double [ %223, %"14.us" ], [ 0.000000e+00, %"4.lr.ph" ]
  %271 = phi double [ %220, %"14.us" ], [ 0.000000e+00, %"4.lr.ph" ]
  %272 = phi double [ %217, %"14.us" ], [ 0.000000e+00, %"4.lr.ph" ]
  %273 = mul i64 %indvars.iv28, 44
  %.sum4.us = add i64 %273, 16
  %274 = getelementptr inbounds i8* %0, i64 %.sum4.us
  %275 = bitcast i8* %274 to float*
  %276 = load float* %275, align 4, !tbaa !0
  %277 = load float* %198, align 4, !tbaa !0
  %278 = fsub float %276, %277
  %279 = fpext float %278 to double
  %.sum6.us = add i64 %273, 20
  %280 = getelementptr inbounds i8* %0, i64 %.sum6.us
  %281 = bitcast i8* %280 to float*
  %282 = load float* %281, align 4, !tbaa !0
  %283 = load float* %200, align 4, !tbaa !0
  %284 = fsub float %282, %283
  %285 = fpext float %284 to double
  %.sum8.us = add i64 %273, 24
  %286 = getelementptr inbounds i8* %0, i64 %.sum8.us
  %287 = bitcast i8* %286 to float*
  %288 = load float* %287, align 4, !tbaa !0
  %289 = load float* %202, align 4, !tbaa !0
  %290 = fsub float %288, %289
  %291 = fpext float %290 to double
  %292 = fmul double %279, %279
  %293 = fmul double %285, %285
  %294 = fadd double %292, %293
  %295 = fmul double %291, %291
  %296 = fadd double %294, %295
  %297 = load float* %204, align 4, !tbaa !0
  %.sum11.us = add i64 %273, 12
  %298 = getelementptr inbounds i8* %0, i64 %.sum11.us
  %299 = bitcast i8* %298 to float*
  %300 = load float* %299, align 4, !tbaa !0
  %301 = fadd float %297, %300
  %302 = fpext float %301 to double
  %303 = fmul double %302, 2.500000e-01
  %304 = fmul double %303, %302
  %305 = fcmp olt double %296, %304
  br i1 %305, label %"7.us", label %"8.us"

"8.us":                                           ; preds = %"6.us"
  %306 = tail call double @sqrt(double %296) nounwind
  %307 = fdiv double 1.000000e+00, %306
  %308 = fmul double %307, %307
  %309 = fmul double %308, %307
  br label %"14.us"

return:                                           ; preds = %"17"
  ret void
}

declare double @sqrt(double) nounwind

define void @blkGrav(%struct.Particle* nocapture %p, i32 %ps, %struct.Particle* nocapture %q, i32 %qs, i32 %iSoftType) nounwind uwtable {
entry:
  %0 = icmp sgt i32 %ps, 0
  br i1 %0, label %"15.preheader.lr.ph", label %return

"15.preheader.lr.ph":                             ; preds = %entry
  %1 = icmp sgt i32 %qs, 0
  %2 = bitcast %struct.Particle* %p to i8*
  %3 = bitcast %struct.Particle* %q to i8*
  br label %"15.preheader"

"4":                                              ; preds = %"14", %"4.lr.ph"
  %indvars.iv = phi i64 [ 0, %"4.lr.ph" ], [ %indvars.iv.next, %"14" ]
  %4 = phi double [ 0.000000e+00, %"4.lr.ph" ], [ %134, %"14" ]
  %5 = phi double [ 0.000000e+00, %"4.lr.ph" ], [ %152, %"14" ]
  %6 = phi double [ 0.000000e+00, %"4.lr.ph" ], [ %149, %"14" ]
  %7 = phi double [ 0.000000e+00, %"4.lr.ph" ], [ %146, %"14" ]
  %8 = mul i64 %indvars.iv, 44
  %.sum4 = add i64 %8, 16
  %9 = getelementptr inbounds i8* %3, i64 %.sum4
  %10 = bitcast i8* %9 to float*
  %11 = load float* %10, align 4, !tbaa !0
  %12 = load float* %209, align 4, !tbaa !0
  %13 = fsub float %11, %12
  %14 = fpext float %13 to double
  %.sum6 = add i64 %8, 20
  %15 = getelementptr inbounds i8* %3, i64 %.sum6
  %16 = bitcast i8* %15 to float*
  %17 = load float* %16, align 4, !tbaa !0
  %18 = load float* %211, align 4, !tbaa !0
  %19 = fsub float %17, %18
  %20 = fpext float %19 to double
  %.sum8 = add i64 %8, 24
  %21 = getelementptr inbounds i8* %3, i64 %.sum8
  %22 = bitcast i8* %21 to float*
  %23 = load float* %22, align 4, !tbaa !0
  %24 = load float* %213, align 4, !tbaa !0
  %25 = fsub float %23, %24
  %26 = fpext float %25 to double
  %27 = fmul double %14, %14
  %28 = fmul double %20, %20
  %29 = fadd double %27, %28
  %30 = fmul double %26, %26
  %31 = fadd double %29, %30
  %.sum10 = add i64 %8, 12
  %32 = getelementptr inbounds i8* %3, i64 %.sum10
  %33 = bitcast i8* %32 to float*
  %34 = load float* %33, align 4, !tbaa !0
  %35 = load float* %215, align 4, !tbaa !0
  %36 = fadd float %34, %35
  %37 = fpext float %36 to double
  switch i32 %iSoftType, label %"9" [
    i32 1, label %"6"
    i32 2, label %"5"
  ]

"5":                                              ; preds = %"4"
  %38 = fmul double %37, 2.500000e-01
  %39 = fmul double %38, %37
  %40 = fadd double %39, %31
  %41 = tail call double @sqrt(double %40) nounwind
  %42 = fdiv double 1.000000e+00, %41
  %43 = fmul double %42, %42
  %44 = fmul double %43, %42
  br label %"14"

"6":                                              ; preds = %"4"
  %45 = fmul double %37, 2.500000e-01
  %46 = fmul double %45, %37
  %47 = fcmp olt double %31, %46
  br i1 %47, label %"7", label %"8"

"7":                                              ; preds = %"6"
  %48 = fmul double %37, %46
  %49 = fdiv double 2.000000e+00, %48
  %50 = fmul double %49, 5.000000e-01
  %51 = fmul double %46, 3.000000e+00
  %52 = fsub double %51, %31
  %53 = fmul double %50, %52
  br label %"14"

"8":                                              ; preds = %"6"
  %54 = tail call double @sqrt(double %31) nounwind
  %55 = fdiv double 1.000000e+00, %54
  %56 = fmul double %55, %55
  %57 = fmul double %56, %55
  br label %"14"

"9":                                              ; preds = %"4"
  %58 = tail call double @sqrt(double %31) nounwind
  %59 = fcmp olt double %58, %37
  br i1 %59, label %"10", label %"13"

"10":                                             ; preds = %"9"
  %60 = fdiv double 2.000000e+00, %37
  %61 = fmul double %58, %60
  %62 = fcmp olt double %61, 1.000000e+00
  br i1 %62, label %"11", label %"12"

"11":                                             ; preds = %"10"
  %63 = fmul double %61, 0x3FE5555555555555
  %64 = fmul double %63, %61
  %65 = fsub double 1.400000e+00, %64
  %66 = fmul double %61, 3.000000e-01
  %67 = fmul double %66, %61
  %68 = fmul double %67, %61
  %69 = fmul double %68, %61
  %70 = fadd double %65, %69
  %71 = fmul double %61, 1.000000e-01
  %72 = fmul double %71, %61
  %73 = fmul double %72, %61
  %74 = fmul double %73, %61
  %75 = fmul double %74, %61
  %76 = fsub double %70, %75
  %77 = fmul double %76, %60
  %78 = fmul double %60, %60
  %79 = fmul double %78, %60
  %80 = fmul double %61, 1.200000e+00
  %81 = fmul double %80, %61
  %82 = fsub double 0x3FF5555555555555, %81
  %83 = fmul double %61, 5.000000e-01
  %84 = fmul double %83, %61
  %85 = fmul double %84, %61
  %86 = fadd double %82, %85
  %87 = fmul double %79, %86
  br label %"14"

"12":                                             ; preds = %"10"
  %88 = fdiv double 1.000000e+00, %58
  %89 = fmul double %88, 0xBFB1111111111111
  %90 = fmul double %61, 0x3FF5555555555555
  %91 = fmul double %90, %61
  %92 = fsub double 1.600000e+00, %91
  %93 = fmul double %61, %61
  %94 = fmul double %93, %61
  %95 = fadd double %92, %94
  %96 = fmul double %61, 3.000000e-01
  %97 = fmul double %96, %61
  %98 = fmul double %97, %61
  %99 = fmul double %98, %61
  %100 = fsub double %95, %99
  %101 = fmul double %61, 0x3FA1111111111111
  %102 = fmul double %101, %61
  %103 = fmul double %102, %61
  %104 = fmul double %103, %61
  %105 = fmul double %104, %61
  %106 = fadd double %100, %105
  %107 = fmul double %106, %60
  %108 = fadd double %89, %107
  %109 = fmul double %89, %88
  %110 = fmul double %109, %88
  %111 = fmul double %60, %60
  %112 = fmul double %111, %60
  %113 = fmul double %61, 3.000000e+00
  %114 = fsub double 0x4005555555555555, %113
  %115 = fmul double %61, 1.200000e+00
  %116 = fmul double %115, %61
  %117 = fadd double %114, %116
  %118 = fmul double %61, 0x3FC5555555555555
  %119 = fmul double %118, %61
  %120 = fmul double %119, %61
  %121 = fsub double %117, %120
  %122 = fmul double %112, %121
  %123 = fadd double %110, %122
  br label %"14"

"13":                                             ; preds = %"9"
  %124 = fdiv double 1.000000e+00, %58
  %125 = fmul double %124, %124
  %126 = fmul double %125, %124
  br label %"14"

"14":                                             ; preds = %"13", %"12", %"11", %"8", %"7", %"5"
  %127 = phi double [ %124, %"13" ], [ %108, %"12" ], [ %77, %"11" ], [ %55, %"8" ], [ %53, %"7" ], [ %42, %"5" ]
  %128 = phi double [ %126, %"13" ], [ %123, %"12" ], [ %87, %"11" ], [ %57, %"8" ], [ %49, %"7" ], [ %44, %"5" ]
  %.sum12 = add i64 %8, 8
  %129 = getelementptr inbounds i8* %3, i64 %.sum12
  %130 = bitcast i8* %129 to float*
  %131 = load float* %130, align 4, !tbaa !0
  %132 = fpext float %131 to double
  %133 = fmul double %132, %127
  %134 = fsub double %4, %133
  %.sum13 = add i64 %8, 40
  %135 = getelementptr inbounds i8* %3, i64 %.sum13
  %136 = bitcast i8* %135 to float*
  %137 = load float* %136, align 4, !tbaa !0
  %138 = fpext float %137 to double
  %139 = load float* %217, align 4, !tbaa !0
  %140 = fpext float %139 to double
  %141 = fmul double %140, %127
  %142 = fsub double %138, %141
  %143 = fptrunc double %142 to float
  store float %143, float* %136, align 4, !tbaa !0
  %144 = fmul double %132, %14
  %145 = fmul double %144, %128
  %146 = fadd double %145, %7
  %147 = fmul double %132, %20
  %148 = fmul double %147, %128
  %149 = fadd double %148, %6
  %150 = fmul double %132, %26
  %151 = fmul double %150, %128
  %152 = fadd double %151, %5
  %.sum15 = add i64 %8, 28
  %153 = getelementptr inbounds i8* %3, i64 %.sum15
  %154 = bitcast i8* %153 to float*
  %155 = load float* %154, align 4, !tbaa !0
  %156 = fpext float %155 to double
  %157 = load float* %217, align 4, !tbaa !0
  %158 = fpext float %157 to double
  %159 = fmul double %158, %14
  %160 = fmul double %159, %128
  %161 = fsub double %156, %160
  %162 = fptrunc double %161 to float
  store float %162, float* %154, align 4, !tbaa !0
  %.sum16 = add i64 %8, 32
  %163 = getelementptr inbounds i8* %3, i64 %.sum16
  %164 = bitcast i8* %163 to float*
  %165 = load float* %164, align 4, !tbaa !0
  %166 = fpext float %165 to double
  %167 = load float* %217, align 4, !tbaa !0
  %168 = fpext float %167 to double
  %169 = fmul double %168, %20
  %170 = fmul double %169, %128
  %171 = fsub double %166, %170
  %172 = fptrunc double %171 to float
  store float %172, float* %164, align 4, !tbaa !0
  %.sum17 = add i64 %8, 36
  %173 = getelementptr inbounds i8* %3, i64 %.sum17
  %174 = bitcast i8* %173 to float*
  %175 = load float* %174, align 4, !tbaa !0
  %176 = fpext float %175 to double
  %177 = load float* %217, align 4, !tbaa !0
  %178 = fpext float %177 to double
  %179 = fmul double %178, %26
  %180 = fmul double %179, %128
  %181 = fsub double %176, %180
  %182 = fptrunc double %181 to float
  store float %182, float* %174, align 4, !tbaa !0
  %indvars.iv.next = add i64 %indvars.iv, 1
  %lftr.wideiv = trunc i64 %indvars.iv.next to i32
  %exitcond = icmp eq i32 %lftr.wideiv, %qs
  br i1 %exitcond, label %"16", label %"4"

"16":                                             ; preds = %"15.preheader", %"14"
  %.lcssa20 = phi double [ %134, %"14" ], [ 0.000000e+00, %"15.preheader" ]
  %.lcssa19 = phi double [ %152, %"14" ], [ 0.000000e+00, %"15.preheader" ]
  %.lcssa18 = phi double [ %149, %"14" ], [ 0.000000e+00, %"15.preheader" ]
  %.lcssa = phi double [ %146, %"14" ], [ 0.000000e+00, %"15.preheader" ]
  %.sum = add i64 %207, 28
  %183 = getelementptr inbounds i8* %2, i64 %.sum
  %184 = bitcast i8* %183 to float*
  %185 = load float* %184, align 4, !tbaa !0
  %186 = fpext float %185 to double
  %187 = fadd double %186, %.lcssa
  %188 = fptrunc double %187 to float
  store float %188, float* %184, align 4, !tbaa !0
  %.sum1 = add i64 %207, 32
  %189 = getelementptr inbounds i8* %2, i64 %.sum1
  %190 = bitcast i8* %189 to float*
  %191 = load float* %190, align 4, !tbaa !0
  %192 = fpext float %191 to double
  %193 = fadd double %192, %.lcssa18
  %194 = fptrunc double %193 to float
  store float %194, float* %190, align 4, !tbaa !0
  %.sum2 = add i64 %207, 36
  %195 = getelementptr inbounds i8* %2, i64 %.sum2
  %196 = bitcast i8* %195 to float*
  %197 = load float* %196, align 4, !tbaa !0
  %198 = fpext float %197 to double
  %199 = fadd double %198, %.lcssa19
  %200 = fptrunc double %199 to float
  store float %200, float* %196, align 4, !tbaa !0
  %.sum3 = add i64 %207, 40
  %201 = getelementptr inbounds i8* %2, i64 %.sum3
  %202 = bitcast i8* %201 to float*
  %203 = load float* %202, align 4, !tbaa !0
  %204 = fpext float %203 to double
  %205 = fadd double %204, %.lcssa20
  %206 = fptrunc double %205 to float
  store float %206, float* %202, align 4, !tbaa !0
  %indvars.iv.next29 = add i64 %indvars.iv28, 1
  %lftr.wideiv30 = trunc i64 %indvars.iv.next29 to i32
  %exitcond31 = icmp eq i32 %lftr.wideiv30, %ps
  br i1 %exitcond31, label %return, label %"15.preheader"

"15.preheader":                                   ; preds = %"16", %"15.preheader.lr.ph"
  %indvars.iv28 = phi i64 [ 0, %"15.preheader.lr.ph" ], [ %indvars.iv.next29, %"16" ]
  %207 = mul i64 %indvars.iv28, 44
  br i1 %1, label %"4.lr.ph", label %"16"

"4.lr.ph":                                        ; preds = %"15.preheader"
  %.sum5 = add i64 %207, 16
  %208 = getelementptr inbounds i8* %2, i64 %.sum5
  %209 = bitcast i8* %208 to float*
  %.sum7 = add i64 %207, 20
  %210 = getelementptr inbounds i8* %2, i64 %.sum7
  %211 = bitcast i8* %210 to float*
  %.sum9 = add i64 %207, 24
  %212 = getelementptr inbounds i8* %2, i64 %.sum9
  %213 = bitcast i8* %212 to float*
  %.sum11 = add i64 %207, 12
  %214 = getelementptr inbounds i8* %2, i64 %.sum11
  %215 = bitcast i8* %214 to float*
  %.sum14 = add i64 %207, 8
  %216 = getelementptr inbounds i8* %2, i64 %.sum14
  %217 = bitcast i8* %216 to float*
  br label %"4"

return:                                           ; preds = %"16", %entry
  ret void
}

define void @umkGrav(%struct.Particle* nocapture %p, i32 %ps, %struct.Particle* nocapture %q, i32 %qs, i32 %iSoftType) nounwind uwtable {
entry:
  %0 = icmp sgt i32 %ps, 0
  br i1 %0, label %"15.preheader.lr.ph", label %return

"15.preheader.lr.ph":                             ; preds = %entry
  %1 = icmp sgt i32 %qs, 0
  %2 = bitcast %struct.Particle* %p to i8*
  %3 = bitcast %struct.Particle* %q to i8*
  br label %"15.preheader"

"4":                                              ; preds = %"14", %"4.lr.ph"
  %indvars.iv = phi i64 [ 0, %"4.lr.ph" ], [ %indvars.iv.next, %"14" ]
  %4 = phi double [ 0.000000e+00, %"4.lr.ph" ], [ %134, %"14" ]
  %5 = phi double [ 0.000000e+00, %"4.lr.ph" ], [ %143, %"14" ]
  %6 = phi double [ 0.000000e+00, %"4.lr.ph" ], [ %140, %"14" ]
  %7 = phi double [ 0.000000e+00, %"4.lr.ph" ], [ %137, %"14" ]
  %8 = mul i64 %indvars.iv, 44
  %.sum4 = add i64 %8, 16
  %9 = getelementptr inbounds i8* %3, i64 %.sum4
  %10 = bitcast i8* %9 to float*
  %11 = load float* %10, align 4, !tbaa !0
  %12 = load float* %170, align 4, !tbaa !0
  %13 = fsub float %11, %12
  %14 = fpext float %13 to double
  %.sum6 = add i64 %8, 20
  %15 = getelementptr inbounds i8* %3, i64 %.sum6
  %16 = bitcast i8* %15 to float*
  %17 = load float* %16, align 4, !tbaa !0
  %18 = load float* %172, align 4, !tbaa !0
  %19 = fsub float %17, %18
  %20 = fpext float %19 to double
  %.sum8 = add i64 %8, 24
  %21 = getelementptr inbounds i8* %3, i64 %.sum8
  %22 = bitcast i8* %21 to float*
  %23 = load float* %22, align 4, !tbaa !0
  %24 = load float* %174, align 4, !tbaa !0
  %25 = fsub float %23, %24
  %26 = fpext float %25 to double
  %27 = fmul double %14, %14
  %28 = fmul double %20, %20
  %29 = fadd double %27, %28
  %30 = fmul double %26, %26
  %31 = fadd double %29, %30
  %.sum10 = add i64 %8, 12
  %32 = getelementptr inbounds i8* %3, i64 %.sum10
  %33 = bitcast i8* %32 to float*
  %34 = load float* %33, align 4, !tbaa !0
  %35 = load float* %176, align 4, !tbaa !0
  %36 = fadd float %34, %35
  %37 = fpext float %36 to double
  switch i32 %iSoftType, label %"9" [
    i32 1, label %"6"
    i32 2, label %"5"
  ]

"5":                                              ; preds = %"4"
  %38 = fmul double %37, 2.500000e-01
  %39 = fmul double %38, %37
  %40 = fadd double %39, %31
  %41 = tail call double @sqrt(double %40) nounwind
  %42 = fdiv double 1.000000e+00, %41
  %43 = fmul double %42, %42
  %44 = fmul double %43, %42
  br label %"14"

"6":                                              ; preds = %"4"
  %45 = fmul double %37, 2.500000e-01
  %46 = fmul double %45, %37
  %47 = fcmp olt double %31, %46
  br i1 %47, label %"7", label %"8"

"7":                                              ; preds = %"6"
  %48 = fmul double %37, %46
  %49 = fdiv double 2.000000e+00, %48
  %50 = fmul double %49, 5.000000e-01
  %51 = fmul double %46, 3.000000e+00
  %52 = fsub double %51, %31
  %53 = fmul double %50, %52
  br label %"14"

"8":                                              ; preds = %"6"
  %54 = tail call double @sqrt(double %31) nounwind
  %55 = fdiv double 1.000000e+00, %54
  %56 = fmul double %55, %55
  %57 = fmul double %56, %55
  br label %"14"

"9":                                              ; preds = %"4"
  %58 = tail call double @sqrt(double %31) nounwind
  %59 = fcmp olt double %58, %37
  br i1 %59, label %"10", label %"13"

"10":                                             ; preds = %"9"
  %60 = fdiv double 2.000000e+00, %37
  %61 = fmul double %58, %60
  %62 = fcmp olt double %61, 1.000000e+00
  br i1 %62, label %"11", label %"12"

"11":                                             ; preds = %"10"
  %63 = fmul double %61, 0x3FE5555555555555
  %64 = fmul double %63, %61
  %65 = fsub double 1.400000e+00, %64
  %66 = fmul double %61, 3.000000e-01
  %67 = fmul double %66, %61
  %68 = fmul double %67, %61
  %69 = fmul double %68, %61
  %70 = fadd double %65, %69
  %71 = fmul double %61, 1.000000e-01
  %72 = fmul double %71, %61
  %73 = fmul double %72, %61
  %74 = fmul double %73, %61
  %75 = fmul double %74, %61
  %76 = fsub double %70, %75
  %77 = fmul double %76, %60
  %78 = fmul double %60, %60
  %79 = fmul double %78, %60
  %80 = fmul double %61, 1.200000e+00
  %81 = fmul double %80, %61
  %82 = fsub double 0x3FF5555555555555, %81
  %83 = fmul double %61, 5.000000e-01
  %84 = fmul double %83, %61
  %85 = fmul double %84, %61
  %86 = fadd double %82, %85
  %87 = fmul double %79, %86
  br label %"14"

"12":                                             ; preds = %"10"
  %88 = fdiv double 1.000000e+00, %58
  %89 = fmul double %88, 0xBFB1111111111111
  %90 = fmul double %61, 0x3FF5555555555555
  %91 = fmul double %90, %61
  %92 = fsub double 1.600000e+00, %91
  %93 = fmul double %61, %61
  %94 = fmul double %93, %61
  %95 = fadd double %92, %94
  %96 = fmul double %61, 3.000000e-01
  %97 = fmul double %96, %61
  %98 = fmul double %97, %61
  %99 = fmul double %98, %61
  %100 = fsub double %95, %99
  %101 = fmul double %61, 0x3FA1111111111111
  %102 = fmul double %101, %61
  %103 = fmul double %102, %61
  %104 = fmul double %103, %61
  %105 = fmul double %104, %61
  %106 = fadd double %100, %105
  %107 = fmul double %106, %60
  %108 = fadd double %89, %107
  %109 = fmul double %89, %88
  %110 = fmul double %109, %88
  %111 = fmul double %60, %60
  %112 = fmul double %111, %60
  %113 = fmul double %61, 3.000000e+00
  %114 = fsub double 0x4005555555555555, %113
  %115 = fmul double %61, 1.200000e+00
  %116 = fmul double %115, %61
  %117 = fadd double %114, %116
  %118 = fmul double %61, 0x3FC5555555555555
  %119 = fmul double %118, %61
  %120 = fmul double %119, %61
  %121 = fsub double %117, %120
  %122 = fmul double %112, %121
  %123 = fadd double %110, %122
  br label %"14"

"13":                                             ; preds = %"9"
  %124 = fdiv double 1.000000e+00, %58
  %125 = fmul double %124, %124
  %126 = fmul double %125, %124
  br label %"14"

"14":                                             ; preds = %"13", %"12", %"11", %"8", %"7", %"5"
  %127 = phi double [ %124, %"13" ], [ %108, %"12" ], [ %77, %"11" ], [ %55, %"8" ], [ %53, %"7" ], [ %42, %"5" ]
  %128 = phi double [ %126, %"13" ], [ %123, %"12" ], [ %87, %"11" ], [ %57, %"8" ], [ %49, %"7" ], [ %44, %"5" ]
  %.sum12 = add i64 %8, 8
  %129 = getelementptr inbounds i8* %3, i64 %.sum12
  %130 = bitcast i8* %129 to float*
  %131 = load float* %130, align 4, !tbaa !0
  %132 = fpext float %131 to double
  %133 = fmul double %132, %127
  %134 = fsub double %4, %133
  %135 = fmul double %132, %14
  %136 = fmul double %135, %128
  %137 = fadd double %136, %7
  %138 = fmul double %132, %20
  %139 = fmul double %138, %128
  %140 = fadd double %139, %6
  %141 = fmul double %132, %26
  %142 = fmul double %141, %128
  %143 = fadd double %142, %5
  %indvars.iv.next = add i64 %indvars.iv, 1
  %lftr.wideiv = trunc i64 %indvars.iv.next to i32
  %exitcond = icmp eq i32 %lftr.wideiv, %qs
  br i1 %exitcond, label %"16", label %"4"

"16":                                             ; preds = %"15.preheader", %"14"
  %.lcssa15 = phi double [ %134, %"14" ], [ 0.000000e+00, %"15.preheader" ]
  %.lcssa14 = phi double [ %143, %"14" ], [ 0.000000e+00, %"15.preheader" ]
  %.lcssa13 = phi double [ %140, %"14" ], [ 0.000000e+00, %"15.preheader" ]
  %.lcssa = phi double [ %137, %"14" ], [ 0.000000e+00, %"15.preheader" ]
  %.sum = add i64 %168, 28
  %144 = getelementptr inbounds i8* %2, i64 %.sum
  %145 = bitcast i8* %144 to float*
  %146 = load float* %145, align 4, !tbaa !0
  %147 = fpext float %146 to double
  %148 = fadd double %147, %.lcssa
  %149 = fptrunc double %148 to float
  store float %149, float* %145, align 4, !tbaa !0
  %.sum1 = add i64 %168, 32
  %150 = getelementptr inbounds i8* %2, i64 %.sum1
  %151 = bitcast i8* %150 to float*
  %152 = load float* %151, align 4, !tbaa !0
  %153 = fpext float %152 to double
  %154 = fadd double %153, %.lcssa13
  %155 = fptrunc double %154 to float
  store float %155, float* %151, align 4, !tbaa !0
  %.sum2 = add i64 %168, 36
  %156 = getelementptr inbounds i8* %2, i64 %.sum2
  %157 = bitcast i8* %156 to float*
  %158 = load float* %157, align 4, !tbaa !0
  %159 = fpext float %158 to double
  %160 = fadd double %159, %.lcssa14
  %161 = fptrunc double %160 to float
  store float %161, float* %157, align 4, !tbaa !0
  %.sum3 = add i64 %168, 40
  %162 = getelementptr inbounds i8* %2, i64 %.sum3
  %163 = bitcast i8* %162 to float*
  %164 = load float* %163, align 4, !tbaa !0
  %165 = fpext float %164 to double
  %166 = fadd double %165, %.lcssa15
  %167 = fptrunc double %166 to float
  store float %167, float* %163, align 4, !tbaa !0
  %indvars.iv.next24 = add i64 %indvars.iv23, 1
  %lftr.wideiv25 = trunc i64 %indvars.iv.next24 to i32
  %exitcond26 = icmp eq i32 %lftr.wideiv25, %ps
  br i1 %exitcond26, label %return, label %"15.preheader"

"15.preheader":                                   ; preds = %"16", %"15.preheader.lr.ph"
  %indvars.iv23 = phi i64 [ 0, %"15.preheader.lr.ph" ], [ %indvars.iv.next24, %"16" ]
  %168 = mul i64 %indvars.iv23, 44
  br i1 %1, label %"4.lr.ph", label %"16"

"4.lr.ph":                                        ; preds = %"15.preheader"
  %.sum5 = add i64 %168, 16
  %169 = getelementptr inbounds i8* %2, i64 %.sum5
  %170 = bitcast i8* %169 to float*
  %.sum7 = add i64 %168, 20
  %171 = getelementptr inbounds i8* %2, i64 %.sum7
  %172 = bitcast i8* %171 to float*
  %.sum9 = add i64 %168, 24
  %173 = getelementptr inbounds i8* %2, i64 %.sum9
  %174 = bitcast i8* %173 to float*
  %.sum11 = add i64 %168, 12
  %175 = getelementptr inbounds i8* %2, i64 %.sum11
  %176 = bitcast i8* %175 to float*
  br label %"4"

return:                                           ; preds = %"16", %entry
  ret void
}

!0 = metadata !{metadata !"alias set 7: float", metadata !1}
!1 = metadata !{metadata !1}
