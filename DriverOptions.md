1/11/2010 Update: Stuart managed to recompile the kernel with a special flag that changes the heap size (?), and ConnTerms now works - we can get the clock signal out on the o-scope, and/or connect it to a digital in line and count the edges.

# Introduction #

After some initial coding with NIDAQmx, and Stuart's trolling of the NI forums, it's becoming clear that some of the c API functions that we want to use crash the system (unless we downgrade to RHEL3).  Let's discuss here the merits of NIDAQmx driver and its alternatives.  The limitations of the driver we go with may or may not have implications for the way we design the rest of the acquisition code (ie - re-writing/adapting may be more difficult than we'd imagine.  And writing something general enough to accommodate all three possibilities may mean taking a performance or features hit - this claim is up for debate).  Of course, running acquisition on an RHEL3 machine also puts limits on the other processes we'll write (such as a video tracker, or gui), if we want to have the option of running those other processes on the same machine that's doing the acquisition. (We'd like to have the option of running those things on other machines of course, but it would be unfortunate if using separate machines was mandatory).

## NIDAQmx 8.0 Linux ##

| **Advantages** | **Disadvantages** |
|:---------------|:------------------|
| kernel module 'works' in CentOS5.5 | ConnTerms crash prevents us from exporting clock signal |
| Successful continuous acq tests |  closed source not gpl    |
| Lots of support online (forums) |  inconsistent sys req (rhel3 vs. 5 in various docs)  |
| Easy to use (no basis for comparison tho) |  ExportClk (sp?) doesn't work |
| auto-threaded chans, thread-safe  | somewhat scary startup segfault when you forget to link with ld |
| Samp clk on board means reliable read intervals | API call failure sometimes hangs system rather than throwing well-behaved error |
| Card manages buffer reads, issues callbacks (convenient for interfacing) | NI tech help instinctively dismissive of unsupported (modern) linux distros/kernels |

Given the on-board timing, RT linux seems unnecessary.

## NIDAQmx Base Linux ##

| Runs under Ubuntu?   |  Similar look&feel to NIDAQmx8 (eg tasks) |
|:---------------------|:------------------------------------------|
|                      | Claims not to be thread-safe              |

Feel free to answer these q's inline, if you research: (thanks)

  * What _is_ base?  Different driver, or just different API? For comparison between NIDAQmx and NIDAQ Base, see http://www.ni.com/dataacquisition/companion_software.htm#driver. NIDAQ Base is not multi-threaded and doesn't seem to support routing of signals.
  * Can it do the clock signal exporting we need?
  * Can it divide down the clock signal and offset it?
  * How does sample timing work?  RT linux necessary, useful?

## comedi/analogy, RTAI/Xenomai ##

| Open source! Runs in Ubuntu 10.04 | Last update was in 2008  |
|:----------------------------------|:-------------------------|
| Support for our card              |                          |
| analogy support for our card, too |                          |
| Pretty active community, discussion boards |                          |

See here for NI's discussion of comedi vs. NIDAQmx:
http://digital.ni.com/public.nsf/allkb/FE65DE3647BC7F1B86256DA4007E55B1

I mention the RT stuff mainly because most of the activity on the discussion forums comes from people making the kernel modules on systems w/ RTAI.  Is RTAI _mandatory_ ?  Is real-time per-se mandatory?  If the latter, Xenomai seems like a more robust RT linux project.  Quick searches on comedi running under Xenomai pointed me to 'analogy' - a fork of the comedi project included in Xenomai.  Does this mean that pretty much all comedi functionality is available _through_ analogy? Will comedi forum questions and sample code be useful to us if we're using analogy instead?

  * http://www.comedi.org/
  * https://mail.gna.org/public/xenomai-help/2006-08/msg00115.html
  * http://www.xenomai.org/index.php/Analogy_general_presentation
  * http://www.xenomai.org/index.php/Main_Pagehttp://www.xenomai.org/index.php/Main_Page

## National Instruments Card vs. Alternative Hardware ##

The NI 6254 is nice because it has 32 AI channels at 16 bits, 1MSPS aggregate rate acquisition, pretty simple multi-card sync within computer through the RTSI cable, and the supposed ability to export several on-board clocks as digital outputs on the card's pins (useful syncing other things going on in other computers).

General Standards has a 64 channel, 16 bit, 200KSPS per channel card that does simultaneous sampling on all channels for $2500 or so (http://www.generalstandards.com/view-products2.php?BD_family=16ai64ssc).  They have driver support for linux, but the API is lower level and not as polished as NIDAQmx. The comedi project lists two General Standards boards, but not the one we are interested in (http://www.comedi.org/hardware.html).

## Find a different sln specifically for computer sync ##

It seems that the most offensive API fn's are the ones we're using to get a hold of the clock signal for inter-computer sync.  We may think about staying with NIDAQmx 8 Linux and the CentOS 5.5 solution, and adding another PCI card (either NI or another vendor, or a Master-8!) for the sole purpose of exporting TTL clock signals (master) and counting those signals (slaves).  Alternatively, by patching the RTSI cable itself, we may be able to get the 80MHz sample clock as TTL?  This would probably void the warranty on the NI cards though.

## Stay with NIDAQmx, Develop under Windows Instead ##

omdb.



Note - this page is meant to be a discussion.  PLEASE feel free to make comments inline, and/or to delete text about ideas that have been ruled out or are plainly untenable.