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
I was now settling in to researching how to code this project, rather than researching what to code it with. But you might be thinking what was taking me
so long. Surely this can't have been that bad.

Well, let's take a look. To make this project work, I need to do a few things:
- Device discovery, where I find all available devices 
- Making the connection, where I pick out the device I want and connect to it
- Managing the connection, which is by far the most complicated part, where I broadcast to and recieve data from with the wii remote
- Interpreting data, where I take the data the wii remote is giving me and make sense of it
- Using the data, where I take what I have and use it to emulate keyboard inputs as I see fit

My current issue is that while I have figured out device discovery, making the connection eludes me. The wii remote has a bluetooth pin, which IS NOT
REQUIRED TO MAKE AND USE A CONNECTION WITH IT, but the fact that it has one was confusing me and a lot of the abstractions I was trying to use to connect
to it. Working out the pin code is something that many people have a lot of ideas on how to do, none of which worked. And believe me, I found the most
obscure of reddit threads in my research. Possibly the most frustrating part of it is that it may have been a sticker that was no longer on my wii remotes
after the years of use.


