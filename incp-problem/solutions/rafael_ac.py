kEndOfChar = '_'

def IsConsonant(c):
  if c == 'a' or c == 'i' or c == 'e' or c == 'o' or c == 'u':
    return False
  return True

def IsCase1(c):
  if c == 'a' or c == 'o' or c == 'u':
    return True
  # Followed by a consonant other than h and y
  if IsConsonant(c) and c != 'h' and c != 'y':
    return True
  # End of a word (represented by '_')
  if c == kEndOfChar:
    return True
  return False

def IsCase2(c):
  if c == 'e' or c == 'i' or c == 'y':
    return True
  return False

def IsCase3(c):
  if c == 'h':
    return True
  return False

def solve(word):
  result = ''
  skip_next = False
  for i in range(len(word)):
    if skip_next == True:
      # Do nothing
      skip_next = False
    elif word[i] == 'c':
      next_char = kEndOfChar
      if i + 1 != len(word):
        next_char = word[i + 1]
      if IsCase1(next_char):
        result += 'k'
      elif IsCase2(next_char):
        result += 's'
      elif IsCase3(next_char):
        result += 'c'
        skip_next = True
    else:
      result += word[i]
  return result

N = int(input())

for i in range(N):
  word = input()
  print(solve(word))
