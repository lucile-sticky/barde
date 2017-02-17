{
    "general" : {
        "site_title": "Sticky playlists"
    },
    "public" : {
        "path" : "/public",
        "host" : "localhost",
        "www_root" : "/var/www"
    },
	"service" : {
		"api" : "http",
		"port" : 8080
	},
	"http" : {
		"script" : "/playlists"
	},
    "localization" : {
        "locales" : [ "en_US.UTF-8" ]
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
