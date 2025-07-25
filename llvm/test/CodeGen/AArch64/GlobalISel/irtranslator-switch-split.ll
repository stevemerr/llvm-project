; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py UTC_ARGS: --version 4
; RUN: llc -global-isel -o - %s | FileCheck %s

target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx14.0.0"

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #0

declare i32 @logg(...)

define i32 @scanfile(i32 %call148, ptr %p) {
; CHECK-LABEL: scanfile:
; CHECK:       ; %bb.0: ; %entry
; CHECK-NEXT:    stp x29, x30, [sp, #-16]! ; 16-byte Folded Spill
; CHECK-NEXT:    .cfi_def_cfa_offset 16
; CHECK-NEXT:    .cfi_offset w30, -8
; CHECK-NEXT:    .cfi_offset w29, -16
; CHECK-NEXT:    mov w8, w0
; CHECK-NEXT:    cmp w0, #1
; CHECK-NEXT:    mov w0, wzr
; CHECK-NEXT:    b.ge LBB0_3
; CHECK-NEXT:  ; %bb.1: ; %entry
; CHECK-NEXT:    cbnz w8, LBB0_7
; CHECK-NEXT:  LBB0_2: ; %common.ret1
; CHECK-NEXT:    ldp x29, x30, [sp], #16 ; 16-byte Folded Reload
; CHECK-NEXT:    ret
; CHECK-NEXT:  LBB0_3: ; %entry
; CHECK-NEXT:    b.eq LBB0_10
; CHECK-NEXT:  ; %bb.4: ; %entry
; CHECK-NEXT:    cmp w8, #2
; CHECK-NEXT:    b.eq LBB0_6
; CHECK-NEXT:  ; %bb.5: ; %entry
; CHECK-NEXT:    cmp w8, #3
; CHECK-NEXT:    b.ne LBB0_2
; CHECK-NEXT:  LBB0_6: ; %sw.bb300
; CHECK-NEXT:    bl _logg
; CHECK-NEXT:    ldp x29, x30, [sp], #16 ; 16-byte Folded Reload
; CHECK-NEXT:    ret
; CHECK-NEXT:  LBB0_7: ; %entry
; CHECK-NEXT:    cmn w8, #2
; CHECK-NEXT:    b.eq LBB0_9
; CHECK-NEXT:  ; %bb.8: ; %entry
; CHECK-NEXT:    cmn w8, #1
; CHECK-NEXT:    b.ne LBB0_2
; CHECK-NEXT:  LBB0_9: ; %sw.bb150
; CHECK-NEXT:    bl _logg
; CHECK-NEXT:    brk #0x1
; CHECK-NEXT:  LBB0_10: ; %sw.bb178
; CHECK-NEXT:    str wzr, [x1]
; CHECK-NEXT:    ldp x29, x30, [sp], #16 ; 16-byte Folded Reload
; CHECK-NEXT:    ret
entry:
  switch i32 %call148, label %common.ret [
    i32 -1, label %sw.bb
    i32 -2, label %sw.bb150
    i32 0, label %sw.bb152
    i32 1, label %sw.bb178
    i32 2, label %sw.bb200
    i32 3, label %sw.bb300
  ]

sw.bb:                                            ; preds = %entry
  %call149 = call i32 (...) @logg()
  unreachable

sw.bb150:                                         ; preds = %entry
  %call151 = call i32 (...) @logg()
  unreachable

sw.bb200:
  %res = call i32 (...) @logg()
  ret i32 %res

sw.bb300:
  %res2 = call i32 (...) @logg()
  ret i32 %res2

common.ret:                                       ; preds = %sw.bb178, %sw.bb152, %entry
  ret i32 0

sw.bb152:                                         ; preds = %entry
  %tobool154.not = icmp eq i32 0, 0
  br label %common.ret

sw.bb178:                                         ; preds = %entry
  store i32 0, ptr %p
  br label %common.ret
}

attributes #0 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
