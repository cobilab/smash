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

## EXAMPLE
In the following instructions we show the procedure to create the information map between human and orangutango chromosome 20 in a Linux environment:

Step 1 (Install Smash, assuming that cmake is installed)
<pre>
wget https://github.com/pratas/smash/archive/master.zip
unzip master.zip
cd smash-master
cmake .
make
</pre>

Step 2 (download the sequences) [the links might change over time]
<pre>wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chr20.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Pongo_abelii/Assembled_chromosomes/seq/pab_ref_P_pygmaeus_2.0.2_chr20.fa.gz
</pre>

Step 3 (unzip and exclude headers)
<pre>zcat hs_ref_GRCh38_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > HS20
zcat pab_ref_P_pygmaeus_2.0.2_chr20.fa.gz |  grep -v ">" | tr -d -c "ACGTN" > PA20</pre>

Step 4 (Run Smash)
<pre>
./smash -v -c 20 -t 1.5 HS20 PA20
</pre>

The last step (4) outputs a SVG image using the respective map under the name: HS20PA20.svg (for custom name use option: -o ANYNAME.svg).



## LICENSE ##

GPL v2. 

For more information:
<pre>http://www.gnu.org/licenses/gpl-2.0.html</pre>

