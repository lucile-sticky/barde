-- MySQL dump 10.13  Distrib 5.5.44, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: playlists
-- ------------------------------------------------------
-- Server version	5.5.44-0+deb8u1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `artist`
--

DROP TABLE IF EXISTS `artist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `artist` (
  `id` int(9) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(50) CHARACTER SET latin1 NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `playlist`
--

DROP TABLE IF EXISTS `playlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `playlist` (
  `id` int(9) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  `image` varchar(100) DEFAULT NULL,
  `description` varchar(500) DEFAULT NULL,
  `publication` date DEFAULT NULL,
  `enabled` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `playlist_comment`
--

DROP TABLE IF EXISTS `playlist_comment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `playlist_comment` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `playlist_id` int(10) unsigned NOT NULL,
  `parent_id` int(10) unsigned DEFAULT NULL,
  `comment` text NOT NULL,
  `creation` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `playlist_id` (`playlist_id`),
  KEY `parent_id` (`parent_id`),
  CONSTRAINT `playlist_comment_ibfk_1` FOREIGN KEY (`playlist_id`) REFERENCES `playlist` (`id`),
  CONSTRAINT `playlist_comment_ibfk_2` FOREIGN KEY (`parent_id`) REFERENCES `playlist_comment` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `playlist_vote`
--

DROP TABLE IF EXISTS `playlist_vote`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `playlist_vote` (
  `playlist_id` int(9) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int(9) unsigned NOT NULL,
  `vote` tinyint(1) NOT NULL DEFAULT '0',
  `creation` datetime NOT NULL,
  PRIMARY KEY (`playlist_id`,`user_id`),
  KEY `user_id` (`user_id`),
  CONSTRAINT `playlist_vote_ibfk_1` FOREIGN KEY (`playlist_id`) REFERENCES `playlist` (`id`),
  CONSTRAINT `playlist_vote_ibfk_2` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `song`
--

DROP TABLE IF EXISTS `song`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `song` (
  `id` int(9) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(45) CHARACTER SET latin1 NOT NULL,
  `artist_id` int(9) unsigned NOT NULL,
  `file` varchar(100) CHARACTER SET latin1 DEFAULT NULL,
  `url` varchar(200) CHARACTER SET latin1 DEFAULT NULL,
  `duration` smallint(4) unsigned DEFAULT NULL,
  `playlist_id` varchar(45) CHARACTER SET latin1 NOT NULL,
  `position` smallint(4) unsigned NOT NULL,
  `proposer_id` int(9) unsigned NOT NULL,
  `show_video` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `proposer_id` (`proposer_id`),
  KEY `artist_id` (`artist_id`),
  CONSTRAINT `song_ibfk_1` FOREIGN KEY (`proposer_id`) REFERENCES `user` (`id`),
  CONSTRAINT `song_ibfk_2` FOREIGN KEY (`artist_id`) REFERENCES `artist` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `song_vote`
--

DROP TABLE IF EXISTS `song_vote`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `song_vote` (
  `song_id` int(9) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int(9) unsigned NOT NULL,
  `vote` tinyint(1) NOT NULL DEFAULT '0',
  `creation` datetime NOT NULL,
  PRIMARY KEY (`song_id`,`user_id`),
  KEY `user_id` (`user_id`),
  CONSTRAINT `song_vote_ibfk_1` FOREIGN KEY (`song_id`) REFERENCES `song` (`id`),
  CONSTRAINT `song_vote_ibfk_2` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `id` int(9) unsigned NOT NULL AUTO_INCREMENT,
  `alias` varchar(20) NOT NULL,
  `username` varchar(30) CHARACTER SET latin1 NOT NULL,
  `password` varchar(100) CHARACTER SET latin1 NOT NULL,
  `level` smallint(3) unsigned NOT NULL DEFAULT '10',
  PRIMARY KEY (`id`),
  UNIQUE KEY `username_UNIQUE` (`username`),
  UNIQUE KEY `alias` (`alias`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-09-18 18:41:53
