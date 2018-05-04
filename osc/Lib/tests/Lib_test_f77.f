      PROGRAM TEST_FORMAT
      IMPLICIT NONE
      INTEGER I,J
      REAL R
      LOGICAL L
      REAL VECT(30)
      I = 314
      J = 315
      R = 3.14
      L = .FALSE.

      VECT( 1) = 1
      VECT( 2) = 2
      VECT( 3) = 3
      VECT( 4) = 4
      VECT( 5) = 5
      VECT( 6) = 6
      VECT( 7) = 7
      VECT( 8) = 8
      VECT( 9) = 9
      VECT(10) = 10

      VECT(11) = 9.1
      VECT(12) = 8.1
      VECT(13) = 7.1
      VECT(14) = 6.1
      VECT(15) = 5.1
      VECT(16) = 4.1
      VECT(17) = 3.1
      VECT(18) = 2.1
      VECT(19) = 1.1

      VECT(20) = 0.1
      VECT(21) = 6.2
      VECT(22) = 4.2
      VECT(23) = 3.2
      VECT(24) = 2.2
      VECT(25) = 1.2
      VECT(26) = 1.2
      VECT(27) = 2.2
      VECT(28) = 3.2
      VECT(29) = 4.2
      VECT(30) = 5.2


C       WRITE (*,'(((G13.5)))') R
C      WRITE (*,'(2(2(i5,2X,A),F10.2))') 
C     +    I,'ab',J,'cd',R, I,'ab',J,'cd',R, I,'ab',J,'cd',R 
C      WRITE (*,'(L10)') L
C      WRITE (*,'(A10,''-'')') 'xxyy'
C      WRITE (*,'(F10.2,1X,/)') R
C      WRITE (*,'(E10.2,1X,I5)') R
C      WRITE (*,'(e10.2,1X,I5)') R
C 
C      WRITE (*,'(5(1X,F13.7))') R,R,R,R,R,R,R,R
C

C      WRITE (*,'(2(5X,10f5.0,/))') (VECT(I),I=1,30)
      WRITE (*,'(5(1X,G13.7))') (VECT(I),I=1,30)

      
      STOP
      END
