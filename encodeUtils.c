#include "encodeUtils.h"

//remember to free result somewhere
char *encodeDnaToProtein(char *dna){
  int size = strlen(dna);
  char *result = (char *) malloc(((size/3) + 1) * sizeof(char));
  for(int i = 0, j = 0; i < size; i+=3, j++){
    if(dna[i] == 'U'){
      if(dna[i+1] == 'U'){
        if(dna[i+2] == 'U'){
          //uuu
          result[j] = 'F';
        } else if(dna[i+2] == 'C'){
          //uuc
          result[j] = 'F';
        } else if(dna[i+2] == 'A'){
          //uua
          result[j] = 'L';
        } else if(dna[i+2] == 'G'){
          //uug
          result[j] = 'L';
        }
      } else if(dna[i+1] == 'C'){
        if(dna[i+2] == 'U'){
          //ucu
          result[j] = 'S';
        } else if(dna[i+2] == 'C'){
          //ucc
          result[j] = 'S';
        } else if(dna[i+2] == 'A'){
          //uca
          result[j] = 'S';
        } else if(dna[i+2] == 'G'){
          //ucg
          result[j] = 'S';
        }
      } else if(dna[i+1] == 'A'){
        if(dna[i+2] == 'U'){
          //uau
          result[j] = 'Y';
        } else if(dna[i+2] == 'C'){
          //uac
          result[j] = 'Y';
        } else if(dna[i+2] == 'A'){
          //uaa
          result[j] = '\0';
        } else if(dna[i+2] == 'G'){
          //uag
          result[j] = '\0';
        }
      } else if(dna[i+1] == 'G'){
        if(dna[i+2] == 'U'){
          //ugu
          result[j] = 'C';
        } else if(dna[i+2] == 'C'){
          //ugc
          result[j] = 'C';
        } else if(dna[i+2] == 'A'){
          //uga
          result[j] = '\0';
        } else if(dna[i+2] == 'G'){
          //ugg
          result[j] = '\0';
        }
      }
    } else if(dna[i] == 'C'){
      if(dna[i+1] == 'U'){
        if(dna[i+2] == 'U'){
          //cuu
          result[j] = 'L';
        } else if(dna[i+2] == 'C'){
          //cuc
          result[j] = 'L';
        } else if(dna[i+2] == 'A'){
          //cua
          result[j] = 'L';
        } else if(dna[i+2] == 'G'){
          //cug
          result[j] = 'L';
        }
      } else if(dna[i+1] == 'C'){
        if(dna[i+2] == 'U'){
          //ccu
          result[j] = 'P';
        } else if(dna[i+2] == 'C'){
          //ccc
          result[j] = 'P';
        } else if(dna[i+2] == 'A'){
          //cca
          result[j] = 'P';
        } else if(dna[i+2] == 'G'){
          //ccg
          result[j] = 'P';
        }
      } else if(dna[i+1] == 'A'){
        if(dna[i+2] == 'U'){
          //cau
          result[j] = 'H';
        } else if(dna[i+2] == 'C'){
          //cac
          result[j] = 'H';
        } else if(dna[i+2] == 'A'){
          //caa
          result[j] = 'Q';
        } else if(dna[i+2] == 'G'){
          //cag
          result[j] = 'Q';
        }
      } else if(dna[i+1] == 'G'){
        if(dna[i+2] == 'U'){
          //cgu
          result[j] = 'R';
        } else if(dna[i+2] == 'C'){
          //cgc
          result[j] = 'R';
        } else if(dna[i+2] == 'A'){
          //cga
          result[j] = 'R';
        } else if(dna[i+2] == 'G'){
          //cgg
          result[j] = 'R';
        }
      }
    } else if(dna[i] == 'A'){
      if(dna[i+1] == 'U'){
        if(dna[i+2] == 'U'){
          //auu
          result[j] = 'I';
        } else if(dna[i+2] == 'C'){
          //auc
          result[j] = 'I';
        } else if(dna[i+2] == 'A'){
          //aua
          result[j] = 'I';
        } else if(dna[i+2] == 'G'){
          //aug
          result[j] = 'M';
        }
      } else if(dna[i+1] == 'C'){
        if(dna[i+2] == 'U'){
          //acu
          result[j] = 'T';
        } else if(dna[i+2] == 'C'){
          //acc
          result[j] = 'T';
        } else if(dna[i+2] == 'A'){
          //aca
          result[j] = 'T';
        } else if(dna[i+2] == 'G'){
          //acg
          result[j] = 'T';
        }
      } else if(dna[i+1] == 'A'){
        if(dna[i+2] == 'U'){
          //aau
          result[j] = 'N';
        } else if(dna[i+2] == 'C'){
          //aac
          result[j] = 'N';
        } else if(dna[i+2] == 'A'){
          //aaa
          result[j] = 'K';
        } else if(dna[i+2] == 'G'){
          //aag
          result[j] = 'K';
        }
      } else if(dna[i+1] == 'G'){
        if(dna[i+2] == 'U'){
          //agu
          result[j] = 'S';
        } else if(dna[i+2] == 'C'){
          //agc
          result[j] = 'S';
        } else if(dna[i+2] == 'A'){
          //aga
          result[j] = 'R';
        } else if(dna[i+2] == 'G'){
          //agg
          result[j] = 'R';
        }
      }
    } else if(dna[i] == 'G'){
      if(dna[i+1] == 'U'){
        if(dna[i+2] == 'U'){
          //guu
          result[j] = 'V';
        } else if(dna[i+2] == 'C'){
          //guc
          result[j] = 'V';
        } else if(dna[i+2] == 'A'){
          //gua
          result[j] = 'V';
        } else if(dna[i+2] == 'G'){
          //gug
          result[j] = 'V';
        }
      } else if(dna[i+1] == 'C'){
        if(dna[i+2] == 'U'){
          //gcu
          result[j] = 'A';
        } else if(dna[i+2] == 'C'){
          //gcc
          result[j] = 'A';
        } else if(dna[i+2] == 'A'){
          //gca
          result[j] = 'A';
        } else if(dna[i+2] == 'G'){
          //gcg
          result[j] = 'A';
        }
      } else if(dna[i+1] == 'A'){
        if(dna[i+2] == 'U'){
          //gau
          result[j] = 'D';
        } else if(dna[i+2] == 'C'){
          //gac
          result[j] = 'D';
        } else if(dna[i+2] == 'A'){
          //gaa
          result[j] = 'E';
        } else if(dna[i+2] == 'G'){
          //gag
          result[j] = 'E';
        }
      } else if(dna[i+1] == 'G'){
        if(dna[i+2] == 'U'){
          //ggu
          result[j] = 'G';
        } else if(dna[i+2] == 'C'){
          //ggc
          result[j] = 'G';
        } else if(dna[i+2] == 'A'){
          //gga
          result[j] = 'G';
        } else if(dna[i+2] == 'G'){
          //ggg
          result[j] = 'G';
        }
      }
    }
  }

  return result;
}

//remember to free result somewhere
char *encodeOneLetter(char *protein){
    int size = (int) strlen(protein);
    int size_one_letter = size/3 + 1;
    char *temp = (char *) malloc(4 * sizeof(char));
    char *result = (char *) malloc(size_one_letter * sizeof(char));
    temp[3] = '\0';

    for(int i = 0, j = 0; i < size, j < size_one_letter; i+=3, j++){
      temp[0] = protein[i];
      temp[1] = protein[i+1];
      temp[2] = protein[i+2];
      if(!strcmp(temp, "Ala") || !strcmp(temp, "ALA")){
        result[j] = 'A';
      } else if(!strcmp(temp, "Arg") || !strcmp(temp, "ARG")){
        result[j] = 'R';
      } else if(!strcmp(temp, "Asp") || !strcmp(temp, "ASP")){
        result[j] = 'D';
      } else if(!strcmp(temp, "Asn") || !strcmp(temp, "ASN")){
        result[j] = 'N';
      } else if(!strcmp(temp, "Cys") || !strcmp(temp, "CYS")){
        result[j] = 'C';
      } else if(!strcmp(temp, "Glu") || !strcmp(temp, "GLU")){
        result[j] = 'E';
      } else if(!strcmp(temp, "Gln") || !strcmp(temp, "GLN")){
        result[j] = 'Q';
      } else if(!strcmp(temp, "Gly") || !strcmp(temp, "GLY")){
        result[j] = 'G';
      } else if(!strcmp(temp, "His") || !strcmp(temp, "HIS")){
        result[j] = 'H';
      } else if(!strcmp(temp, "Ile") || !strcmp(temp, "ILE")){
        result[j] = 'I';
      } else if(!strcmp(temp, "Leu") || !strcmp(temp, "LEU")){
        result[j] = 'L';
      } else if(!strcmp(temp, "Lys") || !strcmp(temp, "LYS")){
        result[j] = 'K';
      } else if(!strcmp(temp, "Met") || !strcmp(temp, "MET")){
        result[j] = 'M';
      } else if(!strcmp(temp, "Phe") || !strcmp(temp, "PHE")){
        result[j] = 'F';
      } else if(!strcmp(temp, "Pro") || !strcmp(temp, "PRO")){
        result[j] = 'P';
      } else if(!strcmp(temp, "Ser") || !strcmp(temp, "SER")){
        result[j] = 'S';
      } else if(!strcmp(temp, "Thr") || !strcmp(temp, "THR")){
        result[j] = 'T';
      } else if(!strcmp(temp, "Trp") || !strcmp(temp, "TRP")){
        result[j] = 'W';
      } else if(!strcmp(temp, "Tyr") || !strcmp(temp, "TYR")){
        result[j] = 'Y';
      } else if(!strcmp(temp, "Val") || !strcmp(temp, "VAL")){
        result[j] = 'V';
      }
    }
    free(temp);
    result[size_one_letter] = '\0';
    return result;
}
