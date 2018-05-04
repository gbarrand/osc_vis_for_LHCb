{
  if( (index($0,"<moc_")!=0) && (index($0,".cpp>")!=0) ) {
    gsub(".cpp",".ic");
  }
  print $0;
}
