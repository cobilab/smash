#!/bin/bash
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chr5.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chr17.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Gorilla_gorilla/Assembled_chromosomes/seq/ggo_ref_gorGor3.1_chr5.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Gorilla_gorilla/Assembled_chromosomes/seq/ggo_ref_gorGor3.1_chr17.fa.gz
zcat hs_ref_GRCh38_chr5.fa.gz | grep -v ">" | tr -d -c "ACGTN" > HS5 ;
zcat hs_ref_GRCh38_chr17.fa.gz | grep -v ">" | tr -d -c "ACGTN" > HS17 ;
zcat ggo_ref_gorGor3.1_chr5.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GG5 ;
zcat ggo_ref_gorGor3.1_chr17.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GG17 ;
rm -f SEP; for((x=1 ; x<1000000 ; ++x)); do print("N") >> SEP; done;
cat HS5 SEP HS17 > HS;
cat GG5 SEP GG17 > GG;
./smash -v -s 1000000 -t 1.5 HS GG ;
