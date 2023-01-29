sciezkaKompWpracy=/home/mateusz/wxWorkspaceLip2022/
projNazwa=Gramont2
projSpakowany=${projNazwa}.tar.gz
projCalaSciezka=${sciezkaKompWpracy}${projNazwa}
folderExclude=${projNazwa}/Debug

zdalnyHost=mateusz@10.36.2.157
zdalnyFolder=/Vista/programowanie/gtkWorkspace

#today=`date +%Y-%m-%d`
cd ${sciezkaKompWpracy}

temp=${projNazwa}Bak
if [ -d "${temp}" ]; then
  # Take action if $DIR exists. #
  rm -rf ${temp}
fi
git clone ${projNazwa} ${temp}
tar cz ${temp} | ssh ${zdalnyHost} "cat | tar xz -C ${zdalnyFolder} --transform=s,${temp},${projNazwa},"
rm -rf ${temp}
#w celu wykonania kompilacji:
#codelite-make -p Gramont2Test -w gtkWorkspace.workspace -c Debug
