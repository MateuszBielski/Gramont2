sciezkaKompWpracy=/home/mateusz/wxWorkspaceLip2022/
projNazwa=Gramont2
projSpakowany=${projNazwa}.tar.gz
projCalaSciezka=${sciezkaKompWpracy}${projNazwa}
folderExclude=${projNazwa}/Debug

zdalnyHost=mateusz@10.36.2.157
zdalnyFolder=/Vista/programowanie/gtkWorkspace

#today=`date +%Y-%m-%d`
cd ${sciezkaKompWpracy}
#klon=${today}${projNazwa}
dat=`date +%F`
projBak=${projNazwa}${dat}Bak
#mv ${projNazwa} ${temp}
git clone ${projNazwa} ${projBak}
tar czf ${projBak}.tar.gz ${projBak}
#tar cz ${projNazwa} | ssh ${zdalnyHost} "cat | tar xz -C ${zdalnyFolder}"
rm -rf ${projBak}
#mv ${temp} ${projNazwa}

#w celu wykonania kompilacji:
#codelite-make -p Gramont2Test -w gtkWorkspace.workspace -c Debug
