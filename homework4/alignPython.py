##Nat Choeypant
##CS474 lab2b align sequences

fileIn = open("seq.txt")

SOURCE = input("enter SOURCE sequence: ")
maxScore = 0
matched = 5
mismatched = -3

##process each line
##calculate the score and update the max score when found
for line in fileIn:
  length = len(line) - 10 - len(SOURCE)
  for i in range (0,length):
    score = 0
    word = line[10+i:10+i+len(SOURCE)]
    mark = ""
    for j in range(0,len(SOURCE)):
      if(SOURCE[j] == word[j]):
        score = score + matched
        mark = mark + "|"
      else:
        score = score + mismatched
        mark = mark + " "
    if(score >= maxScore):
      maxScore = score
      matchedID = line
      totalSpace = i
      maxMark = mark

FINALSOURCE = SOURCE
FINALMARK = maxMark
for x in range(0,totalSpace):
  FINALSOURCE = " " + FINALSOURCE
  FINALMARK = " " + FINALMARK


##print final output
print("SOURCE : "+SOURCE)
print("Highest score : "+str(maxScore))
print("Best match line :"+matchedID[:8])
print(FINALSOURCE)
print(FINALMARK)
print(matchedID[10:])

