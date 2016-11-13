#! /bin/sh

mysqldump playlists -d -uroot -p > playlists.sql

# sreplace (https://sourceforge.net/projects/sreplace/)
sr -e ' AUTO_INCREMENT=[0-9]+' '' playlists.sql
