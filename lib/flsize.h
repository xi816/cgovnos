U32 FileSize(FILE* fl) {
  fseek(fl, 0, SEEK_END);
  U16 flen = ftell(fl);
  fseek(fl, 0, SEEK_SET);
  return flen;
}

