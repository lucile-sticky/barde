{
    "general" : {
        "site_title": "Sticky playlists",
        "media_root_url": "http://localhost/public",
        "upload_root_path": "/tmp"
    },
	"service" : {
		"api" : "http",
		"port" : 8080
	},
	"http" : {
		"script" : "/playlists"
	},
    "localization" : {
        "locales" : [ "fr_FR.UTF-8" ]
    },
    "database" : {
        "connection_string" : "mysql:database=playlists;user=root;password=root;set_charset_name=utf8;@pool_size=1;@use_prepared=on"
    },
    "session" : {
        "expire": "browser",
        "timeout": 604800,
        "location": "server",
        "server":{
            "storage": "files"
        }
    },
    "cache" : {
        "backend" : "thread_shared",
        "limit" : 100
    },
    "logging" : {
        "level" : "debug",
        "stderr" : true
    }
}
