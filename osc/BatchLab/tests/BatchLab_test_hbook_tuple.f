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
      CALL HROPEN(1,'EXAMPLE','EXAMPLE.HB','NQ',2048,ISTAT)
      IF(ISTAT.NE.0)THEN
        PRINT*,'INIT Failed to HROPEN'
        STOP
      ENDIF
C
      RETURN
      END
C************************************************************
      SUBROUTINE BookRWN
C************************************************************
C Row Wise Ntuple
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
C
      INTEGER NVAR
      PARAMETER(NVAR=3)
      CHARACTER*8 CHTAGS(NVAR)
      DATA CHTAGS/'v1','v2','v3'/
C............................................................
      CALL HBOOKN(10,'Ex. RWN Tuple',NVAR,'//MYTUPLE',5000,CHTAGS)
C
      RETURN
      END
C************************************************************
      SUBROUTINE FillRWN
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
C
      INTEGER NVAR
      PARAMETER(NVAR=3)
      REAL VAR(NVAR)
      INTEGER I,J
      REAL X
C............................................................
      DO I=1,1000
        DO J=1,NVAR
           CALL RNDM(x)
           VAR(J) = X
        ENDDO
        CALL HFN(10,VAR)
      ENDDO
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
      INTEGER NVAR
      PARAMETER(NVAR=3)
      REAL VAR
      COMMON/UserVar/VAR(NVAR)
C............................................................
      CALL HBNT(11,'Ex. CWN Tuple',' ') !disk resident
      CALL HBNAME(11,'UserVar',VAR,'VAR(3)')
C
      RETURN
      END
***********************************************************
      Subroutine FillCWN
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
C
      INTEGER NVAR
      PARAMETER(NVAR=3)
      REAL VAR
      COMMON/UserVar/VAR(NVAR)
C
      INTEGER I,J
      REAL X
C............................................................
      DO I=1,1000
        DO J=1,NVAR
           CALL RNDM(x)
           VAR(J) = X
        ENDDO
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
      CALL HREND('EXAMPLE')     ! close HROPEN CHTOP
C
      RETURN
      END
