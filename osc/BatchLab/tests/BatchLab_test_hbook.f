C************************************************************
      PROGRAM HBOOKT
C************************************************************
C  We use the random generators found in Lib/source/FRandom.cxx
C in order to be able to compare between HCL_example.cxx
C and RootBatchLab_test.cxx
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE

      INTEGER NPAWC
      PARAMETER (NPAWC = 1000000)
      INTEGER P
      COMMON /PAWC/ P(NPAWC)
C............................................................
      CALL HLIMIT(NPAWC)

      CALL DIRS()

      CALL XSCALE()

      STOP
      END
C************************************************************
      SUBROUTINE DIRS()
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE

      INTEGER I
      INTEGER ENTRIES
      INTEGER PRNT

      REAL RGAUSS,RBW
C      REAL LIBRGAUSS,LIBRBW
      INTEGER IER,ICYCLE
C............................................................
C      PRNT = 1
      PRNT = 0

      ENTRIES = 1000000

C Create histos directory :
      CALL HCDIR('//PAWC',' ')
      CALL HMDIR('histos',' ')
      CALL HCDIR('histos',' ')

      CALL HMDIR('sub_histos_1',' ')
      CALL HCDIR('sub_histos_1',' ')

      CALL HBOOK1(10,'Gauss',100,-5.,5.,0.)
      DO I=1,ENTRIES
        CALL HFILL(10,RGAUSS(1.,2.),0.,1.4)
      ENDDO

      CALL HBOOK1(20,'BW',100,-5.,5.,0.)
      DO I=1,ENTRIES
        CALL HFILL(20,RBW(0.,1.),0.,2.3)
      ENDDO

      CALL HCDIR('\\',' ')
      CALL HMDIR('sub_histos_2',' ')
      CALL HCDIR('sub_histos_2',' ')

C      CALL HBOOK1(10,'Gauss',100,-5.,5.,0.)
C      DO I=1,ENTRIES
C        CALL HFILL(10,RGAUSS(1.,2.),0.,1.4)
C      ENDDO

      CALL HBPROF(30,'Profile',100,-5.,5.,-2.,2.,' ')
      DO I=1,ENTRIES
        CALL HFILL(30,RGAUSS(1.,2.),RBW(0.,1.),1.)
      ENDDO

      CALL HBOOK2(40,'Gauss/BW',100,-5.,5.,100,-2.,2.,0.)
      DO I=1,ENTRIES
        CALL HFILL(40,RGAUSS(1.,2.),RBW(0.,1.),0.8)
      ENDDO

C      CALL HIDALL(LIST,N)
C      DO I=1,N
C        PRINT *,LIST(I)
C      ENDDO

      CALL NTUPLE()

      IF(PRNT.EQ.1) THEN
        CALL HCDIR('//PAWC/histos/sub_histos_1',' ')
        CALL HPRINT(10)
        CALL HPRINT(20)
C        CALL HCDIR('//PAWC/histos/sub_histos_2',' ')
        CALL HPRINT(30)
        CALL HPRINT(40)
C        CALL HCDIR('//PAWC',' ')
C        CALL HCDIR('tuples',' ')
        CALL HPRINT(100)
      ENDIF

C Store in BatchLab_hbook_test.rz file. 'T' Save complete Tree.
C      CALL HCDIR('//PAWC',' ')
C      CALL HRPUT(0,'BatchLab_test_hbook.hbook','T')

      CALL HCDIR('//PAWC',' ')
      CALL HLDIR('//PAWC','T')

      CALL HROPEN(20,'XXXX','BatchLab_test_hbook.hbook','NQ',1024,ier)
C      CALL HRFILE(20,'XXXX','BatchLab_test_hbook.hbook','NQ')
      IF(IER.NE.0) THEN
        PRINT *,'DIRS : cannot open file'
        RETURN
      ENDIF

      CALL HCDIR('//XXXX',' ')
      CALL HMDIR('histos',' ')
      CALL HCDIR('histos',' ')
      CALL HMDIR('sub_histos_1',' ')
      CALL HMDIR('sub_histos_2',' ')
      CALL HCDIR('//XXXX',' ')
      CALL HMDIR('tuples',' ')

      CALL HCDIR('//PAWC',' ')
      CALL HCDIR('//XXXX',' ')
      ICYCLE = 0
      CALL HROUT(0,ICYCLE,'T')

C      CALL HCDIR('//PAWC/histos/sub_histos_1',' ')
C      CALL HCDIR('//XXXX/histos/sub_histos_1',' ')
C      CALL HROUT(0,ICYCLE,' ')

C      CALL HCDIR('//PAWC/histos/sub_histos_2',' ')
C      CALL HCDIR('//XXXX/histos/sub_histos_2',' ')
C      CALL HROUT(0,ICYCLE,' ')

C      CALL HCDIR('//PAWC/tuples',' ')
C      CALL HCDIR('//XXXX/tuples',' ')
C      CALL HROUT(0,ICYCLE,' ')

      CALL HLDIR('//XXXX','T')

      CALL HREND('XXXX')
      CLOSE(20)

      RETURN
      END
C************************************************************
      SUBROUTINE NTUPLE()
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
      INTEGER ENTRIES
C      REAL LIBRGAUSS,LIBRBW
      REAL RGAUSS,RBW

      REAL ROW(2)

      INTEGER I

      REAL GA,BW
      COMMON/NT200/GA,BW

      CHARACTER*2 TAGS(2)
      DATA TAGS/'ga','bw'/
C............................................................
C Create tuples directory :
      CALL HCDIR('//PAWC',' ')
      CALL HMDIR('tuples',' ')
      CALL HCDIR('tuples',' ')

C Row wise tuple :
      CALL HBOOKN(100,'row wise tuple',2,' ',1000,TAGS)
      ENTRIES = 10000      
      DO I=1,ENTRIES
        ROW(1) = RGAUSS(1.,2.)
        ROW(2) = RBW(0.,1.)
        CALL HFN(100,ROW)
      ENDDO

C Column wise tuple :
      CALL HBNT(200,'column wise tuple',' ')
      CALL HBNAME(200,'NT200',GA,'GA:R,BW:R')
      ENTRIES = 10000      
      DO I=1,ENTRIES
        GA = RGAUSS(1.,2.)
        BW = RBW(0.,1.)
        CALL HFNT(200)
      ENDDO
      RETURN
      END
C************************************************************
      REAL FUNCTION RGAUSS(MEAN,SIGMA)
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
      REAL MEAN,SIGMA

      REAL V1,V2,R,FAC
      REAL RDM
C............................................................
 10   CONTINUE
        V1 = 2.0 * RDM() - 1.0
        V2 = 2.0 * RDM() - 1.0
        R = V1 * V1 + V2 * V2
      IF(R.GT.1.0) GOTO 10
      FAC = SQRT(-2.0 * LOG(R)/R)
      RGAUSS = (V2 * FAC) * SIGMA + MEAN
      RETURN
      END      
C************************************************************
      REAL FUNCTION RBW(MEAN,GAMMA)
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
      REAL MEAN,GAMMA

      REAL R,D
      REAL RDM

      REAL M_PI_2
      DATA M_PI_2 /1.5707963/
C............................................................
      R = 2.0 * RDM() - 1.0
      D = 0.5 * GAMMA * TAN(R * M_PI_2)
      RBW = MEAN + D
      RETURN
      END      
C************************************************************
      REAL FUNCTION RDM()
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE
C CERNLIB RNDM
      REAL RNDM
      REAL DUM
C The FORTRAN RAND not available if using f2c.
C      REAL RAND
C............................................................
C      RDM = RAND()
      RDM = RNDM(DUM)
      RETURN
      END      
C************************************************************
      SUBROUTINE XSCALE()
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IMPLICIT NONE

      INTEGER ENTRIES
      INTEGER I

      REAL RGAUSS
      REAL W,V
      REAL HI
C............................................................
      PRINT *,'Test histo scale with HOPERA :'

      ENTRIES = 1000000

      CALL HBOOK1(60,'Gauss',100,-5.,5.,0.)

      W = 1.4
      DO I=1,ENTRIES
        CALL HFILL(60,RGAUSS(1.,2.),0.,W)
      ENDDO

C      CALL HPRINT(60)
      V = HI(60,50)
      PRINT *,V

      CALL HOPERA(60,'+E',60,60,10.,0.)
      V = HI(60,50)
      PRINT *,V

C      CALL HPRINT(60)

      RETURN
      END
