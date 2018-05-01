#!/bin/bash
list=("added index file","updated readme file", "did some extra calculations","deleted media files","extra code removed")
for i in {1..15}
do
	touch temp$i.txt
	git add -A
	a="Sun may "
	b=$i
	c=" 14:00 2018 +0530"
	export GIT_AUTHOR_DATE=$a$b$c
	export GIT_COMMITTER_DATE=$a$b$c
	git commit -am "`echo ${list[$RANDOM % ${#list[@]} ]}`"
	rm temp$i.txt
	export GIT_AUTHOR_DATE=$a$b$c
	export GIT_COMMITTER_DATE=$a$b$c
	export GIT_AUTHOR_NAME="Dylan Antonio"
	export GIT_AUTHOR_EMAIL="dylan_83000@hotmail.fr"
	export GIT_COMMITTER_NAME="$GIT_AUTHOR_NAME"
	export GIT_COMMITTER_EMAIL="$GIT_AUTHOR_EMAIL"
	git commit -m "Addes some patches"
	git commit -am "`echo ${list[$RANDOM % ${#list[@]} ]}`"
done


