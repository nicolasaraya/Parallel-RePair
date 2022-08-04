# **Parallel RePair**

**Usage:**

Download file in **input** folder: `wget -O ./input/cat.fna.gz https://ftp.ncbi.nlm.nih.gov/genomes/refseq/vertebrate_mammalian/Felis_catus/latest_assembly_versions/GCF_018350175.1_F.catus_Fca126_mat1.0/GCF_018350175.1_F.catus_Fca126_mat1.0_genomic.fna.gz`  

Decompress: `gzip -d ./input/cat.fna.gz`

- `chmod +x compila.sh` 
- `./compila.sh`
- `./exec.out N1 N2`, N1 = Number of Threads, N2 = Option
- `./exec.out N1 N2 N3`, N1= Number of Threads, N2 = Option, N3 = Uniform array of N3 Elements 

- Option :  0 -> Sequential
            1 -> Parallel
            2 -> Sequential and Parallel