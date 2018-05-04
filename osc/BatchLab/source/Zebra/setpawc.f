C************************************************************
      INTEGER*4 FUNCTION SETPAWC()
C************************************************************
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      INTEGER NPAWC
      PARAMETER (NPAWC = 1000000)
      INTEGER P
      COMMON /PAWC/ P(NPAWC)
C
      INTEGER IQUEST
      COMMON/QUEST/IQUEST(100)
C............................................................
      SETPAWC = NPAWC
      RETURN
      END
