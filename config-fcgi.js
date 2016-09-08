{
    "general" : {
        "site_title": "Sticky playlists",
        "media_root_url": "/public",
        "upload_root_path": "/var/lib/playlists/upload"
    },
	"service" : {
		"api" : "fastcgi",
        "socket": "/var/run/playlists.sock"
	},
	"http" : {
		"script" : "/playlists"
	},
    "localization" : {
        "locales" : [ "fr_FR.UTF-8" ]
    },
    "database" : {
        "connection_string" : "mysql:database=playlists;user=root;password=root;set_charset_name=utf8;@pool_size=20;@use_prepared=on"
    },
    "daemon" : {
        "enable" : true,
        "lock" : "/var/run/playlists.pid",
    },
    "session" : {
        "expire" : "browser",
        "timeout" : 36000,
        "location" : "server",
        "server" : {
            "storage" : "files"
        }
    },
    "cache" : {
        "backend" : "thread_shared",
        "limit" : 100
    },
    "logging" : {
        "level" : "debug",
        "file": {
            "name" : "/var/log/playlists.log"
        }
    }
}
