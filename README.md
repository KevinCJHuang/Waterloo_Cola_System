# Question:
	1. Should bottlingPlan yield before the first delivery?
	- Before
  2. When GroupOff chooses a random student id to generate WATCard, should we remove used ids from the list so that no id is pooled twice?
	- Yes
  3. What should be in constructors and what should be at the beginning of main()?
	Constructor: allocating memory,

# Todo List:
- [x] Groupoff: remove assigned giftcards from random pool
- [ ] Check print statement locations
- [x] investigate student.cc: Assignment says "Watch out for the case of a student who only buys one soda using the gift card."
- [x] Error with seed=5, in Appendix 1: Attempt to reset future 0x55f663e3b610 with waiting tasks.
- [x] Delete/How each task ends (See piazza @1056)
- [x] Memory Leak (gift card)

# Appendix
## Appendix 1
uC++ Runtime error (UNIX pid:46717) Attempt to reset future 0x55f663e3b610 with waiting tasks.
Error occurred while executing task Student (0x55f663fbd240).
Stack back trace for: ./soda
(0) ./soda (/*unknown*/)+0x4d5ca [0x55f6637485ca]
(1) ./soda UPP::uBaseFuture<WATCard*>::reset()+0xa1 [0x55f66373541d]
(2) ./soda Future_ISM<WATCard*>::reset()+0x1f [0x55f66373519f]
./runSuite: line 10: 46717 Aborted                 ./soda d 4 > output.txt

# test doc notes
- how to find special cases?
=> use longer tests (e.g., maxPurchases = 80)
## student.cc
### prioritize gift card
- gift card first
G1,0
B1,3

- watcard first
B1,3
G1,0

- watcard first, with no stock
V1
G1,0

### free (advertisement)
- basic advertisement
B1,3
A1,3
B1,1

- advertisement after transfer
B1,1
A1,8
B1,6

- consecutive free
B1,1
A1,8
A1,8
B1,6

### lost
- lost on create
G1,0
L
B1,3

- lost on transfer
G1,0
B1,3
B1,1
L
B1,3

- lost on create, followed by free (advertisement)
G1,0
L
A1,5
B1,3

- lost on transfer, followed by free (advertisement)


- lost twice in a row (or lost on create)
B1,0
L
L
B1,3
