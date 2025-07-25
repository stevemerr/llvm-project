// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py UTC_ARGS: --version 4
// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-feature +zve64x \
// RUN:   -target-feature +zvfbfmin -disable-O0-optnone \
// RUN:   -emit-llvm %s -o - | opt -S -passes=mem2reg | \
// RUN:   FileCheck --check-prefix=CHECK-RV64 %s

#include <riscv_vector.h>

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 2 x i8>, 4) @test_vluxseg4ei8_v_bf16mf4x4_tu(
// CHECK-RV64-SAME: target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 1 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0:[0-9]+]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 2 x i8>, 4) @llvm.riscv.vluxseg4.triscv.vector.tuple_nxv2i8_4t.p0.nxv1i8.i64(target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 1 x i8> [[RS2]], i64 [[VL]], i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[TMP0]]
//
vbfloat16mf4x4_t test_vluxseg4ei8_v_bf16mf4x4_tu(vbfloat16mf4x4_t vd, const __bf16 *rs1, vuint8mf8_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16mf4x4_tu(vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 4 x i8>, 4) @test_vluxseg4ei8_v_bf16mf2x4_tu(
// CHECK-RV64-SAME: target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 2 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 4 x i8>, 4) @llvm.riscv.vluxseg4.triscv.vector.tuple_nxv4i8_4t.p0.nxv2i8.i64(target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 2 x i8> [[RS2]], i64 [[VL]], i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[TMP0]]
//
vbfloat16mf2x4_t test_vluxseg4ei8_v_bf16mf2x4_tu(vbfloat16mf2x4_t vd, const __bf16 *rs1, vuint8mf4_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16mf2x4_tu(vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 4) @test_vluxseg4ei8_v_bf16m1x4_tu(
// CHECK-RV64-SAME: target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 4 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 8 x i8>, 4) @llvm.riscv.vluxseg4.triscv.vector.tuple_nxv8i8_4t.p0.nxv4i8.i64(target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 4 x i8> [[RS2]], i64 [[VL]], i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[TMP0]]
//
vbfloat16m1x4_t test_vluxseg4ei8_v_bf16m1x4_tu(vbfloat16m1x4_t vd, const __bf16 *rs1, vuint8mf2_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16m1x4_tu(vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 16 x i8>, 4) @test_vluxseg4ei8_v_bf16m2x4_tu(
// CHECK-RV64-SAME: target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 8 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 16 x i8>, 4) @llvm.riscv.vluxseg4.triscv.vector.tuple_nxv16i8_4t.p0.nxv8i8.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 8 x i8> [[RS2]], i64 [[VL]], i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[TMP0]]
//
vbfloat16m2x4_t test_vluxseg4ei8_v_bf16m2x4_tu(vbfloat16m2x4_t vd, const __bf16 *rs1, vuint8m1_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16m2x4_tu(vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 2 x i8>, 4) @test_vluxseg4ei8_v_bf16mf4x4_tum(
// CHECK-RV64-SAME: <vscale x 1 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 1 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 2 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv2i8_4t.p0.nxv1i8.nxv1i1.i64(target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 1 x i8> [[RS2]], <vscale x 1 x i1> [[VM]], i64 [[VL]], i64 2, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[TMP0]]
//
vbfloat16mf4x4_t test_vluxseg4ei8_v_bf16mf4x4_tum(vbool64_t vm, vbfloat16mf4x4_t vd, const __bf16 *rs1, vuint8mf8_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16mf4x4_tum(vm, vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 4 x i8>, 4) @test_vluxseg4ei8_v_bf16mf2x4_tum(
// CHECK-RV64-SAME: <vscale x 2 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 2 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 4 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv4i8_4t.p0.nxv2i8.nxv2i1.i64(target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 2 x i8> [[RS2]], <vscale x 2 x i1> [[VM]], i64 [[VL]], i64 2, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[TMP0]]
//
vbfloat16mf2x4_t test_vluxseg4ei8_v_bf16mf2x4_tum(vbool32_t vm, vbfloat16mf2x4_t vd, const __bf16 *rs1, vuint8mf4_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16mf2x4_tum(vm, vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 4) @test_vluxseg4ei8_v_bf16m1x4_tum(
// CHECK-RV64-SAME: <vscale x 4 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 4 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 8 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv8i8_4t.p0.nxv4i8.nxv4i1.i64(target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 4 x i8> [[RS2]], <vscale x 4 x i1> [[VM]], i64 [[VL]], i64 2, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[TMP0]]
//
vbfloat16m1x4_t test_vluxseg4ei8_v_bf16m1x4_tum(vbool16_t vm, vbfloat16m1x4_t vd, const __bf16 *rs1, vuint8mf2_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16m1x4_tum(vm, vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 16 x i8>, 4) @test_vluxseg4ei8_v_bf16m2x4_tum(
// CHECK-RV64-SAME: <vscale x 8 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 8 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 16 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv16i8_4t.p0.nxv8i8.nxv8i1.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 8 x i8> [[RS2]], <vscale x 8 x i1> [[VM]], i64 [[VL]], i64 2, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[TMP0]]
//
vbfloat16m2x4_t test_vluxseg4ei8_v_bf16m2x4_tum(vbool8_t vm, vbfloat16m2x4_t vd, const __bf16 *rs1, vuint8m1_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16m2x4_tum(vm, vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 2 x i8>, 4) @test_vluxseg4ei8_v_bf16mf4x4_tumu(
// CHECK-RV64-SAME: <vscale x 1 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 1 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 2 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv2i8_4t.p0.nxv1i8.nxv1i1.i64(target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 1 x i8> [[RS2]], <vscale x 1 x i1> [[VM]], i64 [[VL]], i64 0, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[TMP0]]
//
vbfloat16mf4x4_t test_vluxseg4ei8_v_bf16mf4x4_tumu(vbool64_t vm, vbfloat16mf4x4_t vd, const __bf16 *rs1, vuint8mf8_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16mf4x4_tumu(vm, vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 4 x i8>, 4) @test_vluxseg4ei8_v_bf16mf2x4_tumu(
// CHECK-RV64-SAME: <vscale x 2 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 2 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 4 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv4i8_4t.p0.nxv2i8.nxv2i1.i64(target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 2 x i8> [[RS2]], <vscale x 2 x i1> [[VM]], i64 [[VL]], i64 0, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[TMP0]]
//
vbfloat16mf2x4_t test_vluxseg4ei8_v_bf16mf2x4_tumu(vbool32_t vm, vbfloat16mf2x4_t vd, const __bf16 *rs1, vuint8mf4_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16mf2x4_tumu(vm, vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 4) @test_vluxseg4ei8_v_bf16m1x4_tumu(
// CHECK-RV64-SAME: <vscale x 4 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 4 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 8 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv8i8_4t.p0.nxv4i8.nxv4i1.i64(target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 4 x i8> [[RS2]], <vscale x 4 x i1> [[VM]], i64 [[VL]], i64 0, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[TMP0]]
//
vbfloat16m1x4_t test_vluxseg4ei8_v_bf16m1x4_tumu(vbool16_t vm, vbfloat16m1x4_t vd, const __bf16 *rs1, vuint8mf2_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16m1x4_tumu(vm, vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 16 x i8>, 4) @test_vluxseg4ei8_v_bf16m2x4_tumu(
// CHECK-RV64-SAME: <vscale x 8 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 8 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 16 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv16i8_4t.p0.nxv8i8.nxv8i1.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 8 x i8> [[RS2]], <vscale x 8 x i1> [[VM]], i64 [[VL]], i64 0, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[TMP0]]
//
vbfloat16m2x4_t test_vluxseg4ei8_v_bf16m2x4_tumu(vbool8_t vm, vbfloat16m2x4_t vd, const __bf16 *rs1, vuint8m1_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16m2x4_tumu(vm, vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 2 x i8>, 4) @test_vluxseg4ei8_v_bf16mf4x4_mu(
// CHECK-RV64-SAME: <vscale x 1 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 1 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 2 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv2i8_4t.p0.nxv1i8.nxv1i1.i64(target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 1 x i8> [[RS2]], <vscale x 1 x i1> [[VM]], i64 [[VL]], i64 1, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 2 x i8>, 4) [[TMP0]]
//
vbfloat16mf4x4_t test_vluxseg4ei8_v_bf16mf4x4_mu(vbool64_t vm, vbfloat16mf4x4_t vd, const __bf16 *rs1, vuint8mf8_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16mf4x4_mu(vm, vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 4 x i8>, 4) @test_vluxseg4ei8_v_bf16mf2x4_mu(
// CHECK-RV64-SAME: <vscale x 2 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 2 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 4 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv4i8_4t.p0.nxv2i8.nxv2i1.i64(target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 2 x i8> [[RS2]], <vscale x 2 x i1> [[VM]], i64 [[VL]], i64 1, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 4 x i8>, 4) [[TMP0]]
//
vbfloat16mf2x4_t test_vluxseg4ei8_v_bf16mf2x4_mu(vbool32_t vm, vbfloat16mf2x4_t vd, const __bf16 *rs1, vuint8mf4_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16mf2x4_mu(vm, vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 8 x i8>, 4) @test_vluxseg4ei8_v_bf16m1x4_mu(
// CHECK-RV64-SAME: <vscale x 4 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 4 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 8 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv8i8_4t.p0.nxv4i8.nxv4i1.i64(target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 4 x i8> [[RS2]], <vscale x 4 x i1> [[VM]], i64 [[VL]], i64 1, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 8 x i8>, 4) [[TMP0]]
//
vbfloat16m1x4_t test_vluxseg4ei8_v_bf16m1x4_mu(vbool16_t vm, vbfloat16m1x4_t vd, const __bf16 *rs1, vuint8mf2_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16m1x4_mu(vm, vd, rs1, rs2, vl);
}

// CHECK-RV64-LABEL: define dso_local target("riscv.vector.tuple", <vscale x 16 x i8>, 4) @test_vluxseg4ei8_v_bf16m2x4_mu(
// CHECK-RV64-SAME: <vscale x 8 x i1> [[VM:%.*]], target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[VD:%.*]], ptr noundef [[RS1:%.*]], <vscale x 8 x i8> [[RS2:%.*]], i64 noundef [[VL:%.*]]) #[[ATTR0]] {
// CHECK-RV64-NEXT:  entry:
// CHECK-RV64-NEXT:    [[TMP0:%.*]] = call target("riscv.vector.tuple", <vscale x 16 x i8>, 4) @llvm.riscv.vluxseg4.mask.triscv.vector.tuple_nxv16i8_4t.p0.nxv8i8.nxv8i1.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[VD]], ptr [[RS1]], <vscale x 8 x i8> [[RS2]], <vscale x 8 x i1> [[VM]], i64 [[VL]], i64 1, i64 4)
// CHECK-RV64-NEXT:    ret target("riscv.vector.tuple", <vscale x 16 x i8>, 4) [[TMP0]]
//
vbfloat16m2x4_t test_vluxseg4ei8_v_bf16m2x4_mu(vbool8_t vm, vbfloat16m2x4_t vd, const __bf16 *rs1, vuint8m1_t rs2, size_t vl) {
  return __riscv_vluxseg4ei8_v_bf16m2x4_mu(vm, vd, rs1, rs2, vl);
}
