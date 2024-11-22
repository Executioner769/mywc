# [Build Your Own wc Tool](https://codingchallenges.fyi/challenges/challenge-wc)

### Steps to use the tool
- Clone this repository
  
   ```bash
   git clone https://github.com/Executioner769/mywc.git
   ```
- Run the `make` command to build the `mywc.c` file
  
  ```bash
  cd mywc
  make
  ```

- Similar to `wc` command `mywc` supports options `[-lwcm]`
### Examples

Print lines in a `test.txt`
```bash
./mywc -l test.txt
    7145 test.txt
```
Print words in a `test.txt`
```bash
./mywc -w test.txt
   58164 test.txt
```
Print bytes in a `test.txt`
```bash
./mywc -c test.txt
  342190 test.txt
```
Print characters in a `test.txt`
```bash
./mywc -m test.txt
  339292 test.txt
```
Default options are -l -w -c
```bash
./mywc test.txt
7145   58164  342190 test.txt
```
Print total if multiple files are provided
```bash
./mywc test.txt mywc.c 
    7145   58164  342190 test.txt
     192     645    4350 mywc.c
    7337   58809  346540 total
```
Invalid option
```bash
./mywc -t test.txt 
./mywc: illegal option -- t
usage: mywc [-lcmw] [file ...]
```
File does not exists
```bash
./mywc file.txt   
./mywc: cannot open file: file.txt
```
Ignore files that cannot be processes
```bash
./mywc file.txt test.txt 
./mywc: cannot open file: file.txt
    7145   58164  342190 test.txt
    7145   58164  342190 total
```


