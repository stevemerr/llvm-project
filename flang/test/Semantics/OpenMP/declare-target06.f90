! RUN: %python %S/../test_errors.py %s %flang_fc1 -fopenmp -fopenmp-version=52
! OpenMP Version 5.1
! Check OpenMP construct validity for the following directives:
! 2.14.7 Declare Target Directive
! When used in an implicit none context.

module test_0
    implicit none
!ERROR: No explicit type declared for 'no_implicit_materialization_1'
!$omp declare target(no_implicit_materialization_1)

!ERROR: No explicit type declared for 'no_implicit_materialization_2'
!$omp declare target link(no_implicit_materialization_2)

!WARNING: The usage of TO clause on DECLARE TARGET directive has been deprecated. Use ENTER clause instead. [-Wopen-mp-usage]
!ERROR: No explicit type declared for 'no_implicit_materialization_3'
!$omp declare target to(no_implicit_materialization_3)

!$omp declare target enter(no_implicit_materialization_3)

INTEGER :: data_int = 10
!$omp declare target(data_int)
end module test_0
