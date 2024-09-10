An application that outputs “ticker lines” 
Using a synchronizing mutex object organize threads so that when one or more instances of the application are started, each thread that outputs a string gets the exclusive right to atomically execute a 2-fold string output operation. 
![sync_threads1](https://github.com/user-attachments/assets/f827cb33-6cab-4715-a659-35bf07e9da3f)
