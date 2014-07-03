# SMASH #

Smash is a completely alignment-free tool to find and visualise genomic rearrangements. The detection is based on conditional exclusive compression, namely using a FCM, also known as Markov model, of high context order (typically 20). For visualization, Smash outputs a SVG image, with an ideogram output architecture, where the patterns are represented with several HSV values (only value varies). The following image, illustrating the information maps between human and chimpanzee for the several chromosomes, depicts an example of the compiled runs with Smash: 

![ScreenShot](/HC.png)

## INSTALLATION ##
Cmake is needed for installation. Use an appropriate packet manager to install cmake:
<pre>sudo apt-get install cmake #LINUX UBUNTU</pre>
<pre>brew install cmake #OSX</pre>
<pre>(or any other)</pre>

After run:
<pre>cmake .</pre>
<pre>make</pre>
to create the binary smash.

## REAL EXAMPLE
In the following instructions we show the procedure to create the information map between human and orangutango chromosome 18:
<pre>wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chr18.fa.gz
dfdf
</pre>

## LICENSE ##

GPL v2. 

For more information:
<pre>http://www.gnu.org/licenses/gpl-2.0.html</pre>

