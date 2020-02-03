code aFunProgram
  
  -- This is aFunProgram, for homework 1.

  function main ()
      var
      c: char
      while(true)
        c = GetCh()
        if (c == 'q')
          break
        endIf
        while(c != '\n')
          printChar(c)
          c = GetCh()
          endWhile 
        printChar('\n')
      endWhile
    endFunction
    

endCode
