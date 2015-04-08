# SMASH #

**Smash is a completely alignment-free method to find and visualise genomic rearrangements**. The detection is based on **conditional exclusive compression**, namely using a **FCM**, also known as Markov model, of high context order (typically 20). The method has been approached with a **tool** (also called Smash). For visualization, Smash outputs a **SVG image**, with an **ideogram** output architecture, where the patterns are represented with several **HSV** values (only value varies). The following image, illustrating the information maps between human and chimpanzee for the several chromosomes, depicts an example: 

![ScreenShot](/HC.png)

## INSTALLATION ##

We provide a binary for each 64 bits operating systems (Linux, OSX, Windows). However, for other purposes, such as source code compilation, use the following installation instructions.
Cmake is needed for installation (http://www.cmake.org/). You can download it directly from http://www.cmake.org/cmake/resources/software.html or use an appropriate packet manager.
In the following instructions we show the procedure to install, compile and create the information map between human and orangutan chromosome 20:

### STEP 1

Download, install smash and resolve conflicts.

#### Linux 
<pre>
sudo apt-get install cmake
wget https://github.com/pratas/smash/archive/master.zip
unzip master.zip
cd smash-master
cmake .
make
</pre>

Alternatively, you can install (without cmake and only for linux) using

<pre>
wget https://github.com/pratas/smash/archive/master.zip
unzip master.zip
cd smash-master
mv Makefile.nix Makefile
make
</pre>

#### OS X
Install brew:
<pre>
ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"
</pre>
only if you do not have it. After type:
<pre>
brew install cmake
brew install wget
brew install gcc48
wget https://github.com/pratas/smash/archive/master.zip
unzip master.zip
cd smash-master
cmake .
make
</pre>
With some versions you might need to create a link to cc or gcc (after the *brew install gcc48* command), namely
<pre>
sudo mv /usr/bin/gcc /usr/bin/gcc-old   # gcc backup
sudo mv /usr/bin/cc /usr/bin/cc-old     # cc backup
sudo ln -s /usr/bin/gcc-4.8 /usr/bin/gcc
sudo ln -s /usr/bin/gcc-4.8 /usr/bin/cc
</pre>
In some versions, the gcc48 is installed over /usr/local/bin, therefore you might need to substitute the last two commands by the following two:
<pre>
sudo ln -s /usr/local/bin/gcc-4.8 /usr/bin/gcc
sudo ln -s /usr/local/bin/gcc-4.8 /usr/bin/cc
</pre>

#### Windows

In windows use cygwin (https://www.cygwin.com/) and make sure that it is included in the installation: cmake, make, zcat, unzip, wget, tr, grep (and any dependencies). If you install the complete cygwin packet then all these will be installed. After, all steps will be the same as in Linux.

### Step 2 
Download the sequences [the links might change over time].

##### Linux and OS X
<pre>
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chr20.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Pongo_abelii/Assembled_chromosomes/seq/pab_ref_P_pygmaeus_2.0.2_chr20.fa.gz
</pre>

### Step 3 
Unzip, exclude headers and filter content.

##### Linux
<pre>
zcat hs_ref_GRCh38_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > HS20
zcat pab_ref_P_pygmaeus_2.0.2_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > PA20
</pre>

##### OS X
<pre>
gzcat hs_ref_GRCh38_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > HS20
gzcat pab_ref_P_pygmaeus_2.0.2_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > PA20
</pre>

### Step 4 ###

Run Smash.

##### Linux and OS X

<pre>
./smash -v -c 20 -t 1.5 HS20 PA20
</pre>

This step outputs a SVG image using the respective map under the name: HS20PA20.svg (for custom name use option: -o ANYNAME.svg). The respective information map is the following (transformed in a png image and rotated 90 degrees to left):

![ScreenShot](/HS20PA20.png)

## USAGE ##

The Smash program have many options in the interface because there are a wide variety of parameters that can be defined by the user. However, for the detection of the arrangements only two are critical, namely context and threshold. Mathematical information about these parameters can be found in the paper.

To see the possible options type
<pre>
./smash
</pre>
or
<pre>
./smash -h
</pre>
These will print the following options:
<pre>
Usage: smash &#60OPTIONS&#62... [FILE] [FILE]      

 -h                  give this help,                 
 -V                  display version number,
 -v                  verbose mode,            
 -f                  force (be sure!),        
                                             
 -c  &#60context&#62       context order (DEF: 20),
 -t  &#60threshold&#62     threshold [0.0,2.0] (DEF: 1.5),
 -m  &#60mSize&#62         minimum block size (DEF: 1000000),
 
 -i                  do not show inversions,
 -n                  do not show regulars,
 -r  &#60ratio&#62         image size ratio (MaxSeq/150),
 -a  &#60alpha&#62         alpha estimator (DEF: 1000),
 -s  &#60seed&#62          seed for random 'N',     
 -w  &#60wSize&#62         window size,
 -wt &#60wType&#62         window type [0|1|2|3] (DEF: 0),
 -d  &#60dSize&#62         sub-sample (DEF: 10000),
 -nd                 do not delete temporary files,
 -wi &#60width&#62         sequence width (DEF: 25),
                                             
 -p  &#60posFile&#62       output positions file,
 -o  &#60outFile&#62       output svg plot file,    
                                             
 [refFile]           reference file,          
 [tarFile]           target file.      
</pre>

By default, Smash has many parameters assigned in order to avoid the estimation, enabling only to set both reference and target files. However, these defaults are only estimated to detect rearrangements in primates. Therefore, for other purposes you might need to adjust context and threshold parameters. Moreover, for custom image maps you might also need to set other parameters, such as width and/or ratio (scale).
Only [refFile] and [TarFile] are mandatory.

#### Options meaning

| Parameters     | Meaning                                                                              |
|----------------|:-------------------------------------------------------------------------------------|
| -h             | It will print the parameters menu (help menu)                                        |
| -V             | It will print the smash version number, license type and authors.                    |
| -v             | It will print progress information such as positions of the patterns, times, etc.    |
| -f             | It will force to write over files already created.                                   |
| -c &#60;context&#62;   | Size of the FCM (Markov) context order (interval [1;28]). Contexts above 14 will be handled with a hash-table, where the implementation is approximately linear in memory relatively to the size of the sequence. When the sequence is very fragmented, or the species are somehow distant, or the sequencing/assembly process has low quality this value show not be very high. |
| -t &#60;threshold&#62; | It will be used to segment the high from the low regions of information content (interval [0;2]). For distant species this value might be slightly below 2 (such as 1.9). |
| -m &#60;mSize&#62;     | Minimum size of the block considered as a valid patters after each segmentation process. Values below 1 Million for primate chromosomes might emerge excessive valid patterns. However for other purposes, such as gene scale analysis, this value should be set almost to 1. |
| -i             | It will not show the information map regarding to inverted pattern regions. |
| -n             | It will not show the information map regarding to regular pattern regions (normal regions). |
| -r &#60;ratio&#62;     | Sets the ratio size of the image. Currently is fixed to 1000000 which is an estimated value to the medium of the primate chromosomes sizes relatively to the medium of the screen resolution. This parameter is not automatically adaptad since a fixed value will bring different size chromosomes to the same scale. Nevertheless, to use it in small sequences, namely bacterial genomes, this parameter might be adjusted. |
| -a &#60;alpha&#62;       | Probabilities estimator. This value relates a linear interpolation between the maximum likelihood estimator and the uniform distribution. This also shows that when the total number of events is large, the estimator behaves as a maximum likelihood estimator. Default value is set to 1000. |
| -s &#60;seed&#62;        | This is a parameter to the pseudo-random generation function. Different seed values ensure different generated values. |
| -w &#60;wSize&#62;       | The window size among with the sub-sampling is calculated automatically, nevertheless this value might be adjusted for special needs. |
| -wt &#60;wType&#62;      | Window filtering type. Types: 0, 1, 2 or 3. Type 0 stands for Hamming, 1 for Hann, 2 for Blackman, while 3 represents a rectangular window. |
| -d &#60;dSize&#62;       | Sub-sampling value. This value among with the window size is calculated automatically. Nevertheless, for special purposes this value might be adjusted. | 
| -wi &#60;width&#62;      | Thickness of the image for each sequence. Default value is set to 25. |
| -p &#60;posFile&#62;     | The positions from all the rearrangements detected on the run. |
| -o &#60;outFile&#62;     | The output SVG image filename. The default uses the concatenation of reference with the target filenames (adding the "svg" extension). Beware: if the files are not in the working directory this might have problems due to several types of characters (such as '/'). |
| [refFile]     | The reference filename. |
| [tarFile]     | The target filename. |

#### Positions file

A file will be created, reporting each rearrangement position, with the following characteristics: type, id, start position, end position, direction.

An example ca be seen below, where columns are separated by a tab (ascii code:9)

|type            |id     |start        | end            | direction |
|:---------------|:------|:------------|:---------------|:----------|
|TARGET          |1      |12890        |  9068115       | 0-regular |
|REFERENCE       |1      |5542700      |  14243450      | 0-regular |
|TARGET          |2      |9100340      |  13134910      | 0-regular |
|REFERENCE       |2      |14243450     |  16350965      | 0-regular |
|REFERENCE       |2      |16383190     |  18129785      | 0-regular |
|TARGET          |3      |13154245     |  18883850      | 0-regular |
|REFERENCE       |3      |18136230     |  19083645      | 0-regular |
|REFERENCE       |3      |19160985     |  23666040      | 0-regular |
|TARGET          |4      |18961190     |  20920470      | 1-inverted|
|REFERENCE       |4      |23840055     |  23975400      | 0-regular |
|REFERENCE       |4      |24001180     |  24639235      | 0-regular |
|REFERENCE       |4      |24697240     |  25754220      | 0-regular |

The first column reports if the regions are in the target or reference sequences and the second column sets an id for each similar region. The third and fourth columns, repectively, indicate the beginning and the end of each similar region, while the last column reports the direction (if was inverted or regular).

## CITATION ##

On using this software/method please cite:

Diogo Pratas, Raquel M. Silva, Armando J. Pinho, Paulo J. S. G. Ferreira. An alignment-free method to find and visualise rearrangements between pairs of DNA sequences. Scientific Reports, 2015. (Accepted)
DOI: doi-to-appear

## ISSUES ##

For any issue let us know at [issues link](https://github.com/pratas/smash/issues).

## LICENSE ##

GPL v2.

For more information:
<pre>http://www.gnu.org/licenses/gpl-2.0.html</pre>

