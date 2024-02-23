# FUCKOTRON

This project started a few weeks ago, when I stumbled upon (code adventure)'s "how a computer works"; which is a  must watch in my opinion; regardless of you knowledge level in microelectronics. i had originally watched it when it came out, along with Ben eater's custom CPU series. it picked my interest, and i was now wondering if i could do something of the sort. after writing a really simple brainfuck interpreter(add link to it in the GitHub)in c++ (which still has a few unresolved bug) during a boring lecture it dawned on me that what i needed to build was a custom CPU meant to interpret brainfuck. After testing a few logic gates simulator, including (code adventure)'s, i settled on Logisim-evolution(add link)

For those who don't know, Brainfuck is an esoteric programming language(link); which is an impractical language whose sole purpose is to test the limit of what it means to be a programming language. Brainfuck is by fare the most famous, and one of the oldest, going back to the 70s (link). it's simple commands set makes it the perfect language for a simple beginner project like this one, considering it only has 8 command :

> \+  - \> < . , \[ \]

And a 30.000  8 bit cell ram; which means that you can store up to 30000 different values; each going from 0 to 255.

I'll explain those command in more detail as I go through the computer design and function. while it might seem that you can hardly build anything useful with such a reduced instruction set, people still managed to make complex program such as (his name)'s fractal thingy.

Thus was born the FUCKOTRON.
![[main.png]]
After two weeks of work, 3 major revision and a full rebuild of most components, i arrived at the final version of this computer, which is represented in fig1.
In total, the final revision; V3; is made of 395 logic gates, detailed in the table below :

| TYPE | N* |
| ---- | ---- |
| AND | 138 |
| XOR | 97 |
| OR | 79 |
| D-FLIPFLOP | 60 |
| NOT | 19 |
| NOR | 2 |
Along with the clock, set to 1Hz; a RAM and ROM module; a DIP switch along with a simple button to input 8 bit number and a TTY screen to print the result. 
___
Building everything as a single module would be unnecessarily time consuming, so the entire build is made from sub assembly, made in turn of logic gates and nested sub assembly. the final version actually looks like this, in it's most simple form.

![[project/FUCKOTRON/v3.png]]
The center component, PATH_DIVIDER, is used to decode instruction provided by the ROM (READ-ONLY-MEMORY), and modify accordingly the RAM (RANDOM-ACCESS-MEMORY)'s values. being comprised of only 8 different instruction, a brf program can be written as an array of 3 bit value, ranging from 0 to 7. Thus, \(\*1) 
> +++\[>+++++<\-]>

becomes :
>0 0 0 6 2 0 0 0 0 0 3 1 7 2 

___
![[rom.png]]
___
![[ADRESSE_MAP.png]]
The computer tracks it's progress along a given program with the ADRESS-MAP block; which is also used to track the current RAM cell; it consist of a simple 16-bit register, made from an array of T-FLIP-FLOP, a simple component capable of remembering a single bit (either a 1 or a 0)
![[x16COUNTER.png]]The register is in turn fed into another component, made from 4 different 4 bit adders which are in turn made from 4 simple 1 bit adder : 
![[x1ADDER.png]]
Adding one to the 16 bit signal is a simple as adding one to the bottom adder, and feeding it's overflow signal into the next adder. Because subtracting number in binary is as easy as adding two together (using the 2's complementary rule); subtracting one is equivalent to  adding the biggest value possible and ignoring the overflow.
![[plyu.png]]
Along with the 16-bit register, this gives use an easy way to advance trough the program
(in both direction) and remember where we are. Combined with a rom module; provided by Logisim; we now have a pretty good ROM architecture.
___
Now onto the RAM !
![[ramarchi.png]]
While it might seem a little more involved than the ROM module, the basis of it is pretty similar, including the ADDRESS_MAP; which is an exact copy, and the RAM module itself provided by Logisim. The address pointer in controlled by the next-cell (\>) and previous-cell (<) instruction, which each advance(in either direction) the cell counter by one. this is achieved by providing current to the NEXTCELL(or PREVIOUSCELL) line, then to the UPDATEREGS line. 
___
To change the value stored at a specific address, we can use the plus-one (+) and minus-one (-) command, with either add or subtract one to the value stored at the address pointer. to achieve that we use the same PLUS-MINUS block used in the address map but modified to use 8 bit number instead of 16. The updated value is then fed into a 8 bit counter, similar to the 16 bit one used in the address map; as a buffer to prevent the computer from entering a loop in which the modified value fed into the ram gets updated again in the same clock cycle. to execute these two commands; we simply supply current to the PLUS (or MINUS) line, then to UPDATEREGS to update the buffer and finally to the UPDATERAM line to well... update the ram.
___
Another way to modify the value stored at the cell register is to directly ask the computer user to input a custom value. it's achieved with the enter-value (,) function. this function is by far the least used but I still implemented it, with as simple 8-value dip switch in which the user can input any 8 bit number he could want; by entering it's value in binary and then pushing a send button. It proved to be a bit difficult to actually implement, as the computer needs to wait for the user's input, and also needs to switch the ram input from the buffer to the dip switch itself.
___
The last of the "regular" function is the simple print-value (.) call; which take the value stored at current cell and print it to a screen using the ASCII character set(set). Logisim provide a simple pseudo-TTY terminal which does just that, so printing a character is as simple a reading the current cell value by providing current to the READ line and then sending current to the screen.
___
After executing one of these simple function, the computer update the ROM register to read the next instruction and get on with the program. Executing an instruction sometime require more than one action; for example; to execute plus-one (+); you need to:
- receive the instruction
- set the PLUS line to 1 for the duration of the action
- set the UPDATE-REGISTER line to 1 for a tick
- set the UPDATE-RAM line to 1 for a tick
- set the NEXT-INSTRUCTION line to 1
- and finally set the UPDATE-ROM line to 1

thankfully, we can do some of those action simultaneously, by starting to read the next instruction while still finishing the current one for example, thus, the computer function on a 4 tick cycle, with 4 clock update needed to execute a simple instruction. the job of synchronizing all of this mess is done by the PATH_DIVIDER
![[PATH_DIVIDER.png]]
it's the heart of the computer and link the ROM to the RAM. But the PATH-DIVIDER serve another function : Dealing with the last two instruction; which are used to create loop. the first one; (\[); jumps the program counter to the matching ending bracket if and only if the current cell is null. one the other end, the ending bracket (]) is used to jump back to the first if and only if the current cell is different to null. because our computer is quite dumb, we have no easy way to know where in the program should we jump. to achieve the instruction; the path divider needs to first establish if the condition to start a loop are met. if they ain't, it simply gets on with it's life and get to the next instruction. otherwise, the computer enters a "jump-state"; i which the clock signal is diverted from the cycle counter to a different one; used exclusively for the loop. in this state the computer continuously check to see if the next (or previous) instruction is and ending bracket (or closing one) and stops there if it is. to account for nested loop; we need to add a "bracket counter"; which tracks count of the number of bracket we've seen. 
___
all of this logic seems pretty simple in hindsight but it all comes to be pretty challenging, especially for a first project. i only learned that I've got micro-electronics classes this semester, in which we'll learn )pretty much everything I've had to figure out here, and more !
this is my first "big" project in a really really long time and i can't wait to get started on something else ! 
___
in the meantime, if you want to test this computer for yourself, you can find all the file on GitHub(add link); along with a suite of test program. to test them, you just need to right click on the ROM and click "Load Image ", then, reset the simulation, launch it, and enjoy !
present in the repo is a simple hello world program along with a more compact one, which take a few minutes more to run. the computer is fare from fast, and i recommend you test everything with the more compact version, but fell free to test the unfolded one to see each and every component working in unison ! iv'e also included a simple c++ script used to translate from brainfuck to 3 bit binary file !
the obvious next step in this endeavor is to actually build it, but i currently lake the funds or knowledge necessary, so maybe at a future date ! my next endeavor is a text formating app to better write my work reports and this blog right here !




(\*1)this program is used to input 14 in the second cell of the ram(from wikilang),i used it extensively to test the \[ and ] command's behavior, which define a simple loop.

stats : 
subassembly: 
v1 : 23 componant
v2 : 15 componant
v3 : 14 componant

gates :
v1 : 500
v2 : 400
v3 : 395
