# SMASH #

Smash is a completely alignment-free tool to find and visualise genomic rearrangements. The detection is based on conditional exclusive compression, namely using a FCM, also known as Markov model, of high context order (typically 20). For visualization, Smash outputs a SVG image, with an ideogram output architecture, where the patterns are represented with several HSV values (only value varies). The following image, illustrating the information maps between human and chimpanzee for the several chromosomes, depicts an example of the compiled runs with Smash: 

![ScreenShot](/HC.png)

## INSTALLATION ##
Cmake is needed for installation (http://www.cmake.org/).
You can download directly from http://www.cmake.org/cmake/resources/software.html or use an appropriate packet manager to install cmake, such as:
<pre>sudo apt-get install cmake #LINUX UBUNTU</pre>
<pre>brew install cmake #OSX</pre>
<pre>(or any other)</pre>

After download Smash, unzip, go to the source folder and type:
<pre>cmake .</pre>
<pre>make</pre>
to create the binary smash.

## EXAMPLE ##
In the following instructions we show the procedure to create the information map between human and orangutango chromosome 20:

### STEP 1 ###

Download, install smash and resolve conflits.

#### Linux #### 
<pre>
sudo apt-get install cmake
wget https://github.com/pratas/smash/archive/master.zip
unzip master.zip
cd smash-master
cmake .
make
</pre>

#### OS X ####
Only if you don't have brew run:
<pre>
ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"
</pre>
An after:
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

### Step 2 ###
Download the sequences [the links might change over time].

#### Linux and OS X ####
<pre>wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chr20.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Pongo_abelii/Assembled_chromosomes/seq/pab_ref_P_pygmaeus_2.0.2_chr20.fa.gz
</pre>

### Step 3 ###
Unzip, exclude headers and filter content.

#### Linux ####
<pre>zcat hs_ref_GRCh38_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > HS20
zcat pab_ref_P_pygmaeus_2.0.2_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > PA20</pre>

#### OS X ####
<pre>gzcat hs_ref_GRCh38_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > HS20
gzcat pab_ref_P_pygmaeus_2.0.2_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > PA20</pre>

### Step 4 ###
Run Smash.

#### Linux, OS X and Windows ####
<pre>
./smash -v -c 20 -t 1.5 HS20 PA20
</pre>

The last step (4) outputs a SVG image using the respective map under the name: HS20PA20.svg (for custom name use option: -o ANYNAME.svg). The respective information map is the following (transformed in a png image and rotated 90 degrees to left):

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
Usage: smash [OPTIONS]... [FILE] [FILE]      
                                             
 -v                  verbose mode            
 -f                  force (be sure!)        
                                             
 -c  [context]       context order (DEF: 20)
 -t  [threshold]     threshold [0.0,2.0] (DEF: 1.5)
                                             
 -m  [mSize]         minimum block size (DEF: 1000000)
 -i                  show only inverted repeats
 -r  [ratio]         image size ratio (DEF: 1000000)
 -a  [alpha]         alpha estimator (DEF: 1000)
 -s  [seed]          seed for random 'N'     
 -w  [wSize]         window size
 -wt [wType]         window type [0|1|2|3] (DEF: 0)
 -d  [dSize]         sub-sample (DEF: 10000)
 -m  [mSize]         minimum block size (DEF: 1000000)
 -wi [width]         sequence width (DEF: 25)
                                             
 -o  [outFile]       output svg plot file    
                                             
 [refFile]           reference file          
 [tarFile]           target file      
</pre>

## REQUESTS / ISSUES ##

For any request or issue please send an email to one of the following authors: {pratas, raquelsilva, ap, pjf} @ua.pt.

## LICENSE ##

GPL v2.

For more information:
<pre>http://www.gnu.org/licenses/gpl-2.0.html</pre>

