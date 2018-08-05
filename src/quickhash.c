
unsigned long int generate_hash_djb2(char msg[], long int msg_length){
    unsigned long int hash = 5381;
    for(int i = 0; 0 < msg_length; i++){
        hash = hash << 5;
        hash += hash;
        hash += msg[i];
    }
    return hash;
}
