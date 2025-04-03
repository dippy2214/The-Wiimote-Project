# The Wiimote Project 🎮
Welcome to the wiimote project, An app I am working on as I study in uni. This app is intended to allow the user to connect to a wii remote and 
use it to emulate keyboard inputs, allowing them to do anything on their PC with a wii remote (providing they have a wii sensor bar - or two 
candles - to use the pointer). This is still in an unrefined state (call it a beta) but feel free to try it out! 

The program already supports connecting to and using the wii remote, pointer and all, but is missing a lot of quality of life features and 
sometimes individual clicks with the wiimote can be janky.

## Key Goals 📒
There are two key goals for this project: 
- Make an app that anyone could use, even a non programmer
- Create it without using any external libraries besides the windows API.

This project is developed using only the windows API because I thought that would be fun for me, and no criticism of this decision will be taken
😇. I want this to be something that everyone can use, but in terms of development this is really a passion project for me to learn about how
the systems involved work on a lower level through a fun lens, so I don't want to just build off somebody elses solution to these problems. 

Full credit to the team working on dolphin emulator, which I used as a valuable learning resource to see their techniques and how these type of 
connections worked. 

## The Development Process 🛠
This project has gone through many iterations, most of which were non functional. I truly dove in at the deep end here, knowing absolutely nothing
about how to make and use a bluetooth connection and with no kind of education on the subject, and at the time of starting only a basic understanding
of how to C++. Over the next 3 years of developing this project, I would learn about how bluetooth connections work, my programming fundamentals 
would improve dramatically (turns out that degree I'm doing is quite helpful!) and I would gain a deep understanding of how the wiimote software
wanted to be communicated with.

But, all of that comes later. To start with, I only had the basics of the original idea, creating some program that let me use the wii remote on my
computer, and neither of the two key goals had really been brought up yet. I wanted a proof of concept, something silly and cool that I could 
show off to my friends. At this stage it was less about learning and more about doing something cool. (if you are only interested in the current
version of the project and more technical details, [click here](https://github.com/dippy2214/The-Wiimote-Project/blob/main/README.md#%EF%B8%8F-a-new-path-forward)

### 🧞 A Whole New World
Picture the scene: I'm just out of high school, done an A level in computer science. A combination of covid and my teacher getting fired mean that 
I essentially taught myself coding to pass. However, my year groups coursework got leaked and didn't contribute to the final grade, meaning we
only got graded on the theory - a potential life saver for me. I managed to get an A in the class and grades good enough to get into Abertay uni,
and now I've moved out of my parents house and to Dundee to study games programming (a subject I realised I enjoyed only through teaching myself
the coding project in my A level). Now I am halfway through first year, I've learned the basics of C++, and now that it's the holiday time I want
a project to keep myself entertained. 

And I just saw a youtube video of someone playing rocket league using a wii remote.

Naturally, my first thought was 'How?'. The whole world of programming was opening up before me, and I was just starting to realise that if I got
good enough at this I could do anything. All the information I need is somewhere on the internet. But I had never seen something like this
before - I had to know. 

My research led me to a piece of software called GlovePie, and so the journey begins.

### ⛳ Humble Beginnings
GlovePIE is an incredible piece of legacy software, created by Carl Kenner who no longer maintains it. Decompilation efforts are currently ongoing,
and all recovered versions are available via a [github repo](https://github.com/GlovePIEPreservation/GlovePIE?tab=readme-ov-file#glovepie-preservation). 

GlovePIE was originally an emulator for VR gloves, but the project grew to support much more hardware - including the nintendo wii remote. it comes
with an IDE and supports a simple scripting language, which can be used to quickly and easily program a system for binding different inputs. 
However, it will not connect the wii remote to the computer itself. For this, I downloaded dolphin emulator, an open source emulator for old
Nintendo consoles, and connected the wii remote through that. With this system, I was able to write keybinds for my wii remote in a simple scripting 
language similar to python and get something up and running quite quickly. In my excitement, I even got a video to share with friends!

[![Watch Here](https://img.youtube.com/vi/BgOgzSvBRzw/0.jpg)](https://www.youtube.com/watch?v=BgOgzSvBRzw&feature=youtu.be "Watch Here!")

    //show/hide glovepie
    if var.run == FALSE then
    var.run = TRUE
    HidePie
    var.hidden = TRUE
    endif
  
    //hide and show glovepie with home button double click
    if ((DoubleClicked(wiimote.Home) and Wiimote.HasClassic == FALSE) or DoubleClicked(Home) or Doubleclicked(wiimote.Classic.Home)) and var.hidden then
    ShowPie
    var.hidden = FALSE
    elseif ((DoubleClicked(wiimote.Home) and Wiimote.HasClassic == FALSE) or DoubleClicked(Home) or Doubleclicked(wiimote.Classic.Home)) and var.hidden == FALSE then
    HidePie
    var.hidden = TRUE
    endif
  
    //wii remote pointer mouse stuff
    wiimote1.led1 = true
  
    mouse.CursorPosX = 1920 - smooth(round((((wiimote.dot1x + wiimote.dot2x)/2)/1012) * 3840), 3)
    mouse.CursorPosY = smooth(round((((wiimote.dot1y + wiimote.dot2y)/2)/800) * 1200), 3)
  
    mouse.LeftButton = wiimote.A and KeepDown(Wiimote.PointerVisible,0.5s)
    mouse.RightButton = wiimote.B and KeepDown(Wiimote.PointerVisible,0.5s)

I have come to love the GlovePIE project, not least because I would now love to make something like that myself. However, while this simple version of
the project satisfied me for a while, it was not enough to shake the idea from my head. I felt as though it had been too easy, and as such that I had
not learned as much as I could have. I wanted *more*.

### 🔂 The First Of Many
I revisited this project that same summer. This time, I knew I wanted to have this whole program in one self contained project. I had big visions 
of supporting all sorts of unconventional input devices, like guitar hero guitars and the DK bongos. But of course, it all started with the wii
remote. I didn't like having to rely on dolphin emulator, so connecting my PC to the wii remote became the first big hurdle, but I also wanted
to recreate what I had achieved through GlovePIE in my own project. And so I created the first of many little test projects.

The first test was made in a C++ project, for one simple reason - it's the language I knew best. I considered using something more adept at 
bluetooth connections, but (perhaps foolishly) chose not to try to learn even more new things at once. I figured that I was already diving head
first into bluetooth connections and a whole world of stuff I didn't understand, and I shouldn't try to learn a whole new language on top of that.
After all, I had barely got the hang of one of them.

### 👨‍💻 Amazing New Technology Is Holding Me Back
At this point I made a decision which I now consider to be a mistake. ChatGPT had just started getting popular. I wanted to try it. And so I did.

What resulted was code which I did not fully understand, which got progressively more conveluted without being understood until I was unable to work
in and edit the project and was forced to start over from scratch. I tried to ask it to explain things to me, but this was no substitute for the 
skills gained from learning enough to make it myself. More to the point, chatGPT had, frankly, no idea what it was doing. seemingly almost as little 
as I myself did. Perhaps programming a connection to a wii remote isn't so common in it's training data (shocker).

I tried in C# as well after this. Still no dice. It simply didn't have the skills required to make this kind of thing worked without much more 
detailed input from me (at which point why not do it myself). However, I had started picking up on some basics of bluetooth and some libraries that
I could look in to. Some noteable picks include a wiimote library on Nuget for C# which could let me interface with the wii remote through C# once 
it was already connected, handling the low level stuff for me in a similar way to GlovePIE. WinRT reared it's head as an option for coding with
bluetooth, as did raw winsock through C++. 

What followed was about 8 failed attempts at finding a way to make this project work. The honest truth was that I didn't know enough to interface with
something which required as much fine detail and specific cases as a wii remote yet. But I was slowly learning, little by little.

### 💪 Getting Better, Slowly But Surely
That summer came and went, without significant progress. However, I did get some little wins. I managed to connect my computer to my Iphone, and could
discover devices now. I revisited the project through the christmas holidays of second year as well, but progress had slowed a lot despite my research.
But these little things were adding up.

I had changed my approach since trying to chatGPT the whole project, and looked through more real world example. I settled onto using the windows API 
in C++, since after reading the actual documentation I had a better idea of what was going on there. I looked into dolphin emulator and how they did 
things, and immediately crumbled looking through such a large codebase for the first time in my life. But the thought was there! I also made more of an
effort to fully read and understand everything written on a website I havent mentioned until now, Wiibrew's [wiimote technical guide](https://www.wiibrew.org/wiki/Wiimote).
This is an incredible community driven resource made by reverse engineering the wii remote's and wii's code, and explains how they work  on a technical
level right down to the binary they will send in different kinds of reports.

I was slowly building more confidence in the systems I was trying to use. I still found chatGPT useful, but more as a search engine to find resources 
or specific parts of the documentation I needed but didn't know to even start looking for. By summer in my third year, I was ready and motivated for a 
new and deeper look at this project.

### 🚶‍♂️ A New Path Forward
I was now settling in to researching how to code this project, rather than researching what to code it with. 'But,' you might be thinking, 'What is taking 
so long.' Surely this can't have been that bad.

Well, let's take a look. To make this project work, I need to do a few things:
- Device discovery, where I find all available devices 
- Making the connection, where I pick out the device I want and connect to it
- Collecting the data, where I broadcast to and recieve data from with the wii remote
- Interpreting data, which is by far the most complicated part, where I take the data the wii remote is giving me and make sense of it
- Using the data, where I take what I have and use it to emulate keyboard inputs as I see fit

My current issue is that while I have figured out device discovery, making the connection eludes me. The wii remote has a bluetooth pin, which IS NOT
REQUIRED TO MAKE AND USE A CONNECTION WITH IT, but the fact that it has one was confusing me and a lot of the abstractions I was trying to use to connect
to it. Working out the pin code is something that many people have a lot of ideas on how to do, none of which worked. And believe me, I found the most
obscure of reddit threads in my research. Possibly the most frustrating part of it is that it may have been a sticker that was no longer on my wii remotes
after the years of use.

This problem is actually part of what influenced me to choose something like the WinAPI over other options that may have been slightly easier to use. It
was the first system that I worked out how to gain the control I needed through, making use of the bluetooth and HID libraries. This did mean redesigning
my systems for the other parts of the process though.

### 🌳 In The Thick Of It
Let's get into the nitty gritty of how my systems work. The wiimote is what's known as a bluetooth-HID device, which means it will operate on the bluetooth
HID protocol. This is valuable to me because windows has both a bluetooth and a HID device library, and once I have done the initial bluetooth connection I
can interact with the device through the HID libraries (which I find easier and which dolphin emulator does). 

#### 🗺 Discovery
Discovery was something I was realtively familiar with now from my experiments. Bluetooth communication works between radios (my computer's bluetooth chip) 
and devices (the wiimote). I select the first (and only) radio to use my computer by default, which lets me store the radio info in an object. If this 
works, I find the first bluetooth device and store it's info. I then iterate through all devices using the BluetoothFindNextDevice function, outputting 
data for my own debugging purposes. Then we query the device name, and if it matches with the names we expect from a wii remote (Nintendo RVL-CNT-01 for a
basic wiimote) then I move on to connection.

#### 📶 Connection
To connect I simply forget and then attach it. Forgetting the device just makes sure that nothing goes wrong if the computer remembers it from last time,
which can cause some weird issues. This may seem like a very simple answer to a problem I struggled with for months. That is because it is. It was this 
easy the whole time, and I had missed it entirely due to poor research, a desire to skip steps in learning and go straight to my fun project and a simple
lack of direction with what to look at. I got so caught up in pairing and the pin number that I didn't put time in to realise how simple it would be to 
just not do that.

Once the bluetooth connection is made the program is then free to transition to HID connection, which ironically involves discovery again on the HID side,
and this time checking the hardware IDs against what I know nintendo's wiimote have. This is something I actually spoke about with someone who worked on 
the wiimote support for dolphin emulator, and I learned that dolphin emulator handles this even worse. Because it needs to support all third party wii 
remotes, it cannot exhaustively fill in hardware IDs for all of them. Therefore, it just tries to use every device as a wii remote and waits around to see 
if it works (for windows. linux just lets you get the HID devices directly from your reference to the bluetooth device 🙂). I think the optimal solution 
to this problem, which I may test and try to add to dolphin myself, is to store the hardware IDs in some sort of dictionary in runtime when you find the 
wiimote as a bluetooth device, and use the same hardware IDs to pick out the HID device you want, but I haven't got around to this yet as I'm excited to 
have things working and want to push on with this project.

#### 👨‍💼 Collecting The Data
This is where the wiibrew wiki page truly came to be the single most valuable resource I found on this journey. The wiimote has a system of output reports,
which act as output modes it will switch to when asked by the radio. The wiki page tells me exactly which binary code to send to the wiimote to get the 
different reports from it, so I started with just the buttons (report 0x30) to test. I created a separate thread to listen to the wiimote and update the data
to store it, and quickly realised that I wanted to refactor the code so that wiimotes were their own object. My early version of this was rushed, and it 
is difficult to architect systems you still don't fully understand, so I didn't waste too much time here. But I did want the wiimote to store it's own last
recieved data so when I wanted to access the buttons I could access the last update the program recieved rather than requesting and waiting for a new 
one. 

After I was satisfied with my systems for this, creating buffers of the correct size and reliably recording correct data, I looked into using some of the 
other reports. I looked at getting accelerometer data and data from the IR sensor, but this is better discussed through how it is interpreted. To collect
this new data it is a simple matter of switching reports and making sure the buffer sizes are prepared, but for any full size output reports the wii remote
uses a 22 byte system.

#### 📊 Interpreting The Data
Interpreting the data was (mostly) ok. The core buttons just flipped bits across 2 bytes, so those were quite simple and easy to understand. The accelerometer
was a more complicated problem, because of how it would change based on the report being used. In my final version of this project I settled on the 0x33
output report, which included the core buttons, accelerometer data and IR sensor data. This meant that the accelerometer data was formatted as 3 unsigned ints,
each getting it's own byte. However, the least significant bit of each of these was stored in a different place, within spare bits within the core button bytes.
I did some bit switching to solve this, but it did trip me up for a while.

The real saga was the IR sensor data. Firstly, there was the matter of turning it on. This meant sending the wiimote from my PC a series of input reports which
corresponded to setting specific parts of it's memory to be settings for sensitivity which I desired. It sounds simple, and the wiki gives a very simple
step by step guide on how to do this correctly, but I had many issues. 

First of all, setting the memory at all. I struggled to correctly interpret this, and dolphin emulator used a complicated factory system to make it's input 
reports which I could not fully understand without a guide. In the end I realised the problem was that I was not telling it the length of the data I wished
to write to it's memory, but let it be known that this problem took me a long time, likely due to my lack of proper debugging methods to see into the memory 
of the wiimote. It may have been worth figuring out how to use the read command, but with how hard I was finding the write it would have been difficult to 
trust it.

Then, I was finding that half the data seemed to be missing. This confused me for ages - I thought I was doing everything correctly. Now, let's take a glance
at Nintendo's development process at this time. The software and hardware teams don't have that close a communication, and when things change from the 
preliminary plans sometimes the updates are slow. Initial versions of the wii remote had it using 4 IR tracking points, but the final version used 2. In 
short, the half of the data I thought was missing was actually me turning the sensitivity of the wii remote low enough (correctly) that it didn't pick up on
anything other than the two IR lights on my sensor bar. This meant that the space dedicated to tracking the extra 2 unnecessary lights was empty. The wiimote
was made to track 4 lights, when the final version only used 2. After realising that I actually had been doing this correctly for weeks and just thought it 
was wrong because I'm silly, I experienced... *mixed* feelings. After some time, though, I settled on being happy about it and continued pushing forwards.
All that was left was to translate the byte data directly into an int and piece together some more of the bits which were scattered around different bytes
(details of this were thankfully available on the wiki). I then created a function to convert this into screen space using the resolution of the IR camera
and my own monitor, and it was good to go!

#### ⌨ Using The Data
Now that I had the numbers and buttons, all the was left was to create a system to convert this to emulating keyboard and mouse inputs. Finally, I was 
seeing the light at the end of the tunnel that was this project. Admittedly, it was a amazingly interesting tunnel with lots of pretty sights along the way,
but after 3 years everyone wants to see some kind of closure and move on to the next phase. I built a system, again using the windows API, to run the press 
key command based off a series of enums so that I could change which key I wanted easily, and then immediately bricked my PC. Turns out, you shouldn't run
PressKey in a while loop without some kind of pause. Lesson learned. I added in a wait to give my program a fixed update rate, making it 60 updates per 
second because I study games programming and that sounded reasonable. This all went pretty smoothly, and by moving the cursor to the pointer position I was
calculating I could even control the mouse. This part of the project actually went pretty smoothly - I was much more familiar with the windows documentation
than I was previously, and I think this was a much simpler task. Most of the time was spent writing the enums. And with that, my functionality was completed.

### 🏅 Finishing Touches
To finish off my work on this project, I made a quick modification to turn it into a windows app. This felt like a more user friendly experience to me, and
I liked making the program work based on button clicks. This required some refactoring of the code to make it fit a program loop and not softlock everything,
but was not too complex a change.

### 🏁 Final Thoughts And Conclusions
This is perhaps my favourite thing that I have ever made. Throughout the 3 years since the start of this project that have passed as I do this write up, I 
have learned so much about my independant research skills, my ability to achieve anything if I am willing to put in the time to look into it, the different
levels of depth that I can look in to for my projects and how to make use of both complex documentation and navigate large codebases like that of dolphin
emulator. I have had an incredible time, and in the future I would love to revisit this project to refactor the backend to a cleaner and more robust state 
and give it a nicer GUI, and maybe even to release it as an open source project for anyone to work on (although I suspect that would be more work than I 
realise). This has given me so much to think about and bring to all projects going forwards, and I've loved every minute of working on this.



