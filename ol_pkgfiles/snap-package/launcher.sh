#! /bin/sh

datadir=$SNAP/data
userdir=$SNAP_USER_COMMON/config
userdirlink=$SNAP_USER_DATA/.olc
serverfile=servers.lst
inifile=ol.ini
exename=$SNAP/ol.x86.linux.bin

mkdir -p $userdir || exit

ln -sf $userdir $userdirlink || exit

if [ ! -r $userdir/$serverfile ]
then
	cp -p $datadir/$serverfile $userdir/ || exit
	#echo "new $userdir/$serverfile $(ls -l $userdir/$serverfile)"
fi

if [ -z "$1" ]
then
	config="main"
else
	config="$1"
fi

mkdir -p $userdir/$config || exit

if [ ! -r $userdir/$config/$inifile ]
then
	cp -p $datadir/$inifile $userdir/$config/ || exit
	#echo "new $inifile $(ls -l $userdir/$config/$inifile)"
fi

if [ "$(grep ^#data_dir $userdir/$config/$inifile)" != "#data_dir = $datadir" ]
then
	sed -i "s|^#data_dir.*$|#data_dir = $datadir|g" $userdir/$config/$inifile || exit
	#echo "[$(grep ^#data_dir $userdir/$config/$inifile)]"
fi

if [ "$(grep ^#browser $userdir/$config/$inifile)" != "#browser = xdg-open" ]
then
	sed -i "s|^#browser.*$|#browser = xdg-open|g" $userdir/$config/$inifile || exit
	#echo "[$(grep ^#browser $userdir/$config/$inifile)]"
fi

exec "$exename" "$config"
