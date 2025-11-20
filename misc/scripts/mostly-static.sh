# vim:ft=bash

mkdir obj
sources=($(find src -type f -name \*.cpp -printf '%P '))

for n in ${sources[@]}; do
    arname="src.$(echo $n | perl -pe 's/\//./g').o"
    
    $CXX $CXXFLAGS -I.build -Iinclude -c src/$n -o ./obj/$arname
    

done