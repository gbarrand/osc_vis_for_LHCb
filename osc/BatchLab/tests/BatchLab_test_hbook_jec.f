C************************************************************
      PROGRAM BATCHLABTST
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
C............................................................
      CALL INIT
      CALL BookCWN
      CALL FillCWN
      CALL Finalize

      STOP
      END
C************************************************************
      SUBROUTINE INIT
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
C
      INTEGER IQUEST
      COMMON/QUEST/IQUEST(100)
C
      INTEGER NWPAWC
      PARAMETER(NWPAWC=1500000)
      REAL PAW
      COMMON/PAWC/PAW(NWPAWC)
C
      INTEGER LREC,ISTAT
C............................................................
      CALL HLIMIT(NWPAWC)
C
      IQUEST(10) = 65000        ! maximum number of records
C
      LREC       = 2048         ! record Length (1024: default)
C     option N: new, Q: to change the Quest(10) value
      CALL HROPEN(1,'BATCHLABJEC','BATCHLABJEC.HBOOK','NQ',2048,ISTAT)
      IF(ISTAT.NE.0)THEN
        PRINT*,'INIT Failed to HROPEN'
        STOP
      ENDIF
C
      RETURN
      END
C************************************************************
      Subroutine BookCWN
C************************************************************
C Column Wise Ntuple
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
C
      INTEGER I
C      REAL P1,P2,P3,P4,P5,P6
      REAL*8 P1,P2,P3,P4,P5,P6
      COMMON/UserVar2/I,P1,P2,P3,P4,P5,P6
C............................................................
      CALL HBNT(11,'Ex. CWN Tuple',' ') !disk resident
C      CALL HBNAME(11,'UserVar',I,'I:I,P1:R,P2:R,P3:R,P4:R,P5:R,P6:R')
      CALL HBNAME(11,'UserVar',I,
     + 'I:I,P1:R*8,P2:R*8,P3:R*8,P4:R*8,P5:R*8,P6:R*8')
C
      RETURN
      END
***********************************************************
      Subroutine FillCWN
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
C
      INTEGER I
C      REAL P1,P2,P3,P4,P5,P6
      REAL*8 P1,P2,P3,P4,P5,P6
      COMMON/UserVar2/I,P1,P2,P3,P4,P5,P6
C
      INTEGER INDEX
      REAL M_PI
C............................................................
      M_PI = 3.1415926535897931160E0
      DO INDEX=1,100000
        I = INDEX
        P1 =     M_PI * I;
        P2 = 2 * M_PI * I;
        P3 = 3 * M_PI * I;
        P4 = 4 * M_PI * I;
        P5 = 5 * M_PI * I;
        P6 = 6 * M_PI * I;
        CALL HFNT(11)
      ENDDO
C
      RETURN
      END
C************************************************************
      Subroutine Finalize
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
C
      INTEGER ICYCLE
C............................................................
      CALL HROUT(0,ICYCLE,' ')  ! save all
      CALL HREND('BATCHLABJEC')     ! close HROPEN CHTOP
C
      RETURN
      END
