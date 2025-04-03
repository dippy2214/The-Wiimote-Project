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
show off to my friends. At this stage it was less about learning and more about doing something cool.

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

### ⛳ Small Beginnings
GlovePIE is an incredible piece of legacy software, created by Carl Kenner who no longer maintains it. Decompilation efforts are currently ongoing,
and all recovered versions are available via a [github repo](https://github.com/GlovePIEPreservation/GlovePIE?tab=readme-ov-file#glovepie-preservation). 

GlovePIE was originally an emulator for VR gloves, but the project grew to support much more hardware - including the nintendo wii remote. it comes
with an IDE and supports a simple scripting language, which can be used to quickly and easily program a system for binding different inputs. 
However, it will not connect the wii remote to the computer itself. For this, I downloaded dolphin emulator, an open source emulator for old
Nintendo consoles, and connected the wii remote through that. With this system, I was able to write keybinds for my wii remote in a simple scripting 
language similar to python and get something up and running quite quickly. In my excitement, I even got a video to share with friends!

<a href="https://youtu.be/BgOgzSvBRzw">
  <img src="https://i.ytimg.com/vi/BgOgzSvBRz/oar2.jpg" alt="Watch the video" width="600" height="337">
</a>






