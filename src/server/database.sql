-- MySQL dump 10.13  Distrib 5.1.53-MariaDB, for pc-linux-gnu (x86_64)
--
-- Host: localhost    Database: fallen
-- ------------------------------------------------------
-- Server version	5.1.53-MariaDB-log

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
-- Table structure for table `artifact_types`
--

DROP TABLE IF EXISTS `artifact_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `artifact_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Artifact Type Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Artifact Type Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Game Artifact Types';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `artifact_types`
--

LOCK TABLES `artifact_types` WRITE;
/*!40000 ALTER TABLE `artifact_types` DISABLE KEYS */;
INSERT INTO `artifact_types` VALUES (1,'qwe','qwe');
/*!40000 ALTER TABLE `artifact_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `artifacts`
--

DROP TABLE IF EXISTS `artifacts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `artifacts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `gid` int(11) NOT NULL DEFAULT '0',
  `title` mediumtext NOT NULL COMMENT 'Artifact Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Artifact Body',
  `image` mediumtext NOT NULL,
  `unit` int(11) NOT NULL DEFAULT '0',
  `sellcost` decimal(10,0) NOT NULL DEFAULT '0',
  `buycost` decimal(10,0) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 COMMENT='Game Artifacts';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `artifacts`
--

LOCK TABLES `artifacts` WRITE;
/*!40000 ALTER TABLE `artifacts` DISABLE KEYS */;
INSERT INTO `artifacts` VALUES (1,0,'Пенный Пивас','{\"src\":\"01.png\"}','',0,'0','0'),(2,0,'Сладкий Сухарик','{\"src\":\"02.png\"}','',0,'0','0'),(3,0,'Плюшка Латунная','{\"src\":\"03.png\"}','',0,'0','0'),(4,0,'Меч Кладенец','{\"src\":\"04.png\"}','',0,'0','0'),(5,0,'Щит Бабуина','{\"src\":\"05.png\"}','',0,'0','0');
/*!40000 ALTER TABLE `artifacts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_identity`
--

DROP TABLE IF EXISTS `auth_identity`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_identity` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `login` varchar(255) NOT NULL DEFAULT '' COMMENT 'User Login',
  `password` char(40) NOT NULL DEFAULT '' COMMENT 'User Password',
  `email` varchar(255) NOT NULL DEFAULT '' COMMENT 'User Email',
  `first_name` varchar(255) NOT NULL DEFAULT '' COMMENT 'User First Name',
  `second_name` varchar(255) NOT NULL DEFAULT '' COMMENT 'User Second Name',
  `user_rights` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'User Permissons',
  `activation` char(40) NOT NULL DEFAULT '' COMMENT 'Email Activation Code',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 COMMENT='User Authentification Data';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_identity`
--

LOCK TABLES `auth_identity` WRITE;
/*!40000 ALTER TABLE `auth_identity` DISABLE KEYS */;
INSERT INTO `auth_identity` VALUES (1,'blacksmith','916174c4971e177df8cc6ba20988469824b2cc07','ray_n@pisem.net','Виктор','Дудочкин',0,''),(2,'test','a94a8fe5ccb19ba61c4c0873d391e987982fbbd3','test@gogoo.ru','Тестер','Тестер',8388607,''),(3,'test1','b444ac06613fc8d63795be9ad0beaf55011936ac','test1@inovica.ru','test1','test1',511,''),(4,'test2','109f4b3c50d7b0df729d299bc6f8e9ef9066971f','','','',0,''),(5,'test3','3ebfa301dc59196f18593c45e519287a23297589','','','',0,''),(6,'','da39a3ee5e6b4b0d3255bfef95601890afd80709','','','',0,''),(7,'','da39a3ee5e6b4b0d3255bfef95601890afd80709','','','',0,'');
/*!40000 ALTER TABLE `auth_identity` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `bag`
--

DROP TABLE IF EXISTS `bag`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bag` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Bag Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Bag Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Personage Items';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `bag`
--

LOCK TABLES `bag` WRITE;
/*!40000 ALTER TABLE `bag` DISABLE KEYS */;
INSERT INTO `bag` VALUES (1,'qwe','qwe');
/*!40000 ALTER TABLE `bag` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `clan_users`
--

DROP TABLE IF EXISTS `clan_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clan_users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Clan Users';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `clan_users`
--

LOCK TABLES `clan_users` WRITE;
/*!40000 ALTER TABLE `clan_users` DISABLE KEYS */;
/*!40000 ALTER TABLE `clan_users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `clans`
--

DROP TABLE IF EXISTS `clans`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clans` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Clan Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Clan Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Clans Data';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `clans`
--

LOCK TABLES `clans` WRITE;
/*!40000 ALTER TABLE `clans` DISABLE KEYS */;
INSERT INTO `clans` VALUES (1,'qwe','qwe');
/*!40000 ALTER TABLE `clans` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `client_forms`
--

DROP TABLE IF EXISTS `client_forms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_forms` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Form Name',
  `body` text NOT NULL COMMENT 'Form Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=15 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `client_forms`
--

LOCK TABLES `client_forms` WRITE;
/*!40000 ALTER TABLE `client_forms` DISABLE KEYS */;
INSERT INTO `client_forms` VALUES (1,'chat-window','{\"chat-window\":{\"element\":\"div\",\"className\":\"rounded shadowed chat-window bordered\",\"childs\":{\"img25\":{\"element\":\"img\",\"className\":\"bgimage\",\"src\":\"/img/bg.jpg\"},\"div81\":{\"element\":\"div\",\"title\":\"Example uncommon two-column layout\",\"className\":\"row boxsizingBorder\",\"style\":{\"paddingLeft\":\"20px\"},\"childs\":{\"div64\":{\"element\":\"div\",\"style\":{\"cssFloat\":\"left\",\"position\":\"absolute\",\"right\":\"160px\",\"left\":\"0px\",\"bottom\":\"10px\",\"top\":\"0px\"},\"childs\":{\"labelMainChannel\":{\"element\":\"span\",\"className\":\"label\",\"style\":{\"padding\":\"4px\"},\"childs\":{\"a65\":{\"element\":\"a\",\"href\":\"#\",\"style\":{\"marginRight\":\"4px\",\"cursor\":\"pointer\"},\"onclick\":\"getMainMessages\",\"text\":\"Main Channel\"},\"newCountMainChannel\":{\"element\":\"span\",\"className\":\"label important\",\"style\":{\"display\":\"none\"}}}},\"labelChannel\":{\"element\":\"span\",\"className\":\"label\",\"style\":{\"padding\":\"4px\"},\"childs\":{\"a85\":{\"element\":\"a\",\"href\":\"#\",\"style\":{\"marginRight\":\"4px\",\"cursor\":\"pointer\"},\"onclick\":\"getChannelMessages\",\"text\":\"Channels\"},\"newCountChannel\":{\"element\":\"span\",\"className\":\"label important\",\"style\":{\"display\":\"none\"}}}},\"labelMessages\":{\"element\":\"span\",\"className\":\"label\",\"style\":{\"padding\":\"4px\"},\"childs\":{\"a49\":{\"element\":\"a\",\"href\":\"#\",\"style\":{\"marginRight\":\"4px\",\"cursor\":\"pointer\"},\"onclick\":\"getUserMessages\",\"text\":\"Messages\"},\"newCountMessages\":{\"element\":\"span\",\"className\":\"label important\",\"style\":{\"display\":\"none\"}}}},\"cmessages\":{\"element\":\"ul\",\"className\":\"boxsizingBorder\",\"style\":{\"width\":\"100%\",\"height\":\"120px\",\"bottom\":\"0px\",\"overflowY\":\"auto\",\"marginLeft\":\"2px\",\"paddingLeft\":\"2px\",\"border\":\"1px solid #CCCCCC\"}},\"div16\":{\"element\":\"div\",\"style\":{\"marginRight\":\"0px\"},\"childs\":{\"cmessage\":{\"element\":\"input\",\"style\":{\"position\":\"absolute\",\"width\":\"150px\",\"bottom\":\"-10px\",\"overflowY\":\"auto\",\"left\":\"2px\",\"right\":\"10px\",\"minHeight\":\"30px\"},\"onkeypress\":\"chatSend\"}}}}},\"div13\":{\"element\":\"div\",\"style\":{\"cssFloat\":\"right\",\"marginLeft\":\"10px\",\"width\":\"150px\"},\"childs\":{\"labelUsers\":{\"element\":\"span\",\"className\":\"label\",\"style\":{\"padding\":\"4px\"},\"childs\":{\"a7\":{\"element\":\"a\",\"href\":\"#\",\"style\":{\"marginRight\":\"4px\",\"cursor\":\"pointer\"},\"onclick\":\"getChannelUsers\",\"text\":\"Users\"},\"newCountUsers\":{\"element\":\"span\",\"className\":\"label important\",\"style\":{\"display\":\"none\"}}}},\"labelChallenge\":{\"element\":\"span\",\"className\":\"label\",\"style\":{\"padding\":\"4px\"},\"childs\":{\"a79\":{\"element\":\"a\",\"href\":\"#\",\"style\":{\"marginRight\":\"4px\",\"cursor\":\"pointer\"},\"onclick\":\"getChallenge\",\"text\":\"Challenge\"},\"newCountChallenge\":{\"element\":\"span\",\"className\":\"label important\",\"style\":{\"display\":\"none\"}}}},\"citems\":{\"element\":\"select\",\"name\":\"multiSelect\",\"size\":\"9\",\"className\":\"large\",\"style\":{\"minHeight\":\"160px\",\"width\":\"100%\"},\"onclick\":\"opponent\"}}}}}}}}'),(2,'login-window','{\"login-window\":{\"element\":\"div\",\"className\":\"bordered rounded shadowed\",\"style\":{\"position\":\"absolute\",\"width\":\"220px\",\"left\":\"50%\",\"top\":\"50%\",\"marginLeft\":\"-110px\",\"padding\":\"10px\"},\"childs\":{\"div23\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/titles.png\') no-repeat center\",\"padding\":\"10px\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\",\"textAlign\":\"center\"},\"childs\":{\"b3\":{\"element\":\"b\",\"text\":\"ENTER GAME\"}}},\"img4\":{\"element\":\"img\",\"className\":\"bgimage\",\"src\":\"/img/bg.jpg\"},\"llogin\":{\"element\":\"span\",\"style\":{\"display\":\"block\"},\"text\":\"Login\"},\"user_login\":{\"element\":\"input\",\"className\":\"span4\"},\"lpassword\":{\"element\":\"span\",\"style\":{\"display\":\"block\"},\"text\":\"Password\"},\"user_password\":{\"element\":\"input\",\"className\":\"span4\",\"style\":{\"display\":\"block\",\"marginBottom\":\"10px\"}},\"registerBtn\":{\"element\":\"button\",\"className\":\"btn\",\"style\":{\"width\":\"104px\",\"marginRight\":\"5px\"},\"onclick\":\"showRegister\",\"text\":\"Register\"},\"loginBtn\":{\"element\":\"button\",\"className\":\"btn primary\",\"style\":{\"width\":\"104px\",\"marginLeft\":\"5px\"},\"onclick\":\"login\",\"text\":\"Login\"}}}}'),(3,'duel-launcher','{\"duel-launcher\":{\"element\":\"div\",\"className\":\"rounded shadowed\",\"style\":{\"position\":\"fixed\",\"width\":\"220px\",\"left\":\"735px\",\"top\":\"0%\",\"display\":\"none\",\"background\":\"#ffffff url(\'/img/duel.png\') no-repeat left top\",\"backgroundPosition\":\"10px 10px\",\"padding\":\"10px\",\"minHeight\":\"148px\",\"verticalAlign\":\"top\"},\"childs\":{\"attackUser\":{\"element\":\"div\",\"style\":{\"position\":\"relative\",\"left\":\"100px\",\"width\":\"100px\",\"textAlign\":\"center\",\"display\":\"none\"},\"childs\":{\"h274\":{\"element\":\"h2\",\"text\":\"Attack?\"},\"userToAttack\":{\"element\":\"p\",\"className\":\"label important\",\"text\":\"asd\"},\"challengeDuel\":{\"element\":\"a\",\"href\":\"#\",\"className\":\"btn warning\",\"onclick\":\"challenge\",\"text\":\"Challenge\"},\"acceptChallenge\":{\"element\":\"a\",\"href\":\"#\",\"className\":\"btn warning\",\"onclick\":\"acceptChallenge\",\"style\":{\"display\":\"none\"},\"text\":\"Fight\"},\"br77\":{\"element\":\"br\"},\"rejectChallenge\":{\"element\":\"a\",\"href\":\"#\",\"className\":\"btn warning\",\"onclick\":\"rejectChallenge\",\"style\":{\"display\":\"none\"},\"text\":\"Run Away\"}}}}}}'),(4,'shop-launcher','{\"shop-launcher\":{\"element\":\"div\",\"className\":\"rounded shadowed\",\"style\":{\"position\":\"fixed\",\"width\":\"220px\",\"left\":\"490px\",\"top\":\"0%\",\"background\":\"none repeat scroll 0% 0% rgb(255, 255, 255)\",\"padding\":\"10px\",\"minHeight\":\"148px\",\"display\":\"none\"},\"childs\":{\"shopIcon\":{\"element\":\"img\",\"src\":\"/img/shop.png\",\"onclick\":\"toggleShop\"}}}}'),(5,'mentor-launcher','{\"mentor-launcher\":{\"element\":\"div\",\"className\":\"rounded shadowed\",\"style\":{\"position\":\"fixed\",\"background\":\"none repeat scroll 0% 0% rgb(255, 255, 255)\",\"top\":\"0px\",\"right\":\"0px\",\"padding\":\"10px\",\"display\":\"none\",\"minHeight\":\"148px\",\"minWidth\":\"220px\"},\"childs\":{\"mentorIcon\":{\"element\":\"img\",\"src\":\"/img/mentor.png\",\"onclick\":\"toggleMentor\"}}}}'),(6,'inventory-window','{\"inventory-window\":{\"element\":\"div\",\"className\":\"rounded shadowed\",\"style\":{\"position\":\"fixed\",\"background\":\"none repeat scroll 0% 0% rgb(255, 255, 255)\",\"top\":\"173px\",\"right\":\"0px\",\"bottom\":\"212px\",\"padding\":\"10px\",\"minHeight\":\"148px\",\"width\":\"220px\",\"display\":\"none\"},\"childs\":{\"personageImg\":{\"element\":\"img\",\"src\":\"/img/boy.png\",\"onclick\":\"toggleInventory\"},\"inventoryItems\":{\"element\":\"div\",\"style\":{\"position\":\"inherit\",\"top\":\"283px\",\"bottom\":\"222px\",\"padding\":\"2px\",\"width\":\"215px\",\"overflowY\":\"scroll\"},\"childs\":{\"div25\":{\"element\":\"div\",\"className\":\"cell\",\"text\":\"Блок 1\"}}}}}}'),(7,'shop-window','{\"shop-window\":{\"element\":\"div\",\"className\":\"rounded shadowed\",\"style\":{\"position\":\"fixed\",\"background\":\"none repeat scroll 0% 0% rgb(255, 255, 255)\",\"top\":\"173px\",\"right\":\"0px\",\"bottom\":\"212px\",\"padding\":\"10px\",\"minHeight\":\"148px\",\"width\":\"220px\",\"display\":\"none\"},\"childs\":{\"personageImg\":{\"element\":\"img\",\"src\":\"/img/shop.png\",\"onclick\":\"toggleShop\"},\"shopItems\":{\"element\":\"div\",\"style\":{\"position\":\"inherit\",\"top\":\"283px\",\"bottom\":\"222px\",\"padding\":\"2px\",\"width\":\"215px\",\"overflowY\":\"scroll\"},\"childs\":{\"div86\":{\"element\":\"div\",\"className\":\"cell\",\"text\":\"Блок 1\"}}}}}}'),(8,'ratio-window','{\"ratio-window\":{\"element\":\"div\",\"className\":\"rounded shadowed\",\"style\":{\"position\":\"fixed\",\"width\":\"220px\",\"left\":\"0%\",\"display\":\"none\",\"top\":\"346px\",\"bottom\":\"175px\",\"background\":\"none repeat scroll 0% 0% rgb(255, 255, 255)\",\"padding\":\"10px\",\"minHeight\":\"148px\"},\"text\":\"Ratio\",\"childs\":{\"ratioItems\":{\"element\":\"div\",\"style\":{\"position\":\"inherit\",\"top\":\"356px\",\"bottom\":\"185px\",\"padding\":\"2px\",\"width\":\"215px\",\"overflowY\":\"scroll\"},\"childs\":{\"div51\":{\"element\":\"div\",\"className\":\"cell\",\"text\":\"Блок 1\"}}}}}}'),(9,'news-window','{\"news-window\":{\"element\":\"div\",\"className\":\"rounded shadowed\",\"style\":{\"position\":\"fixed\",\"width\":\"220px\",\"display\":\"none\",\"background\":\"none repeat scroll 0% 0% rgb(255, 255, 255)\",\"bottom\":\"0px\",\"left\":\"0px\",\"padding\":\"10px\",\"minHeight\":\"148px\"},\"childs\":{\"newsItems\":{\"element\":\"div\",\"style\":{\"position\":\"inherit\",\"bottom\":\"10px\",\"padding\":\"2px\",\"width\":\"215px\",\"overflowY\":\"scroll\",\"minHeight\":\"140px\"},\"childs\":{\"div13\":{\"element\":\"div\",\"className\":\"cell\",\"text\":\"Блок 1\"}}}}}}'),(10,'best-window','{\"best-window\":{\"element\":\"div\",\"className\":\"rounded bordered shadowed control-panel\",\"childs\":{\"img69\":{\"element\":\"img\",\"className\":\"bgimage\",\"src\":\"/img/bg.jpg\"},\"stats\":{\"element\":\"div\",\"childs\":{\"personageIcon\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/iAvatarM.png\') no-repeat center\",\"cssFloat\":\"left\"},\"onclick\":\"togglePersonageTab\",\"childs\":{\"personageIcon\":{\"element\":\"img\",\"src\":\"/img/CharPanelBg.png\",\"onclick\":\"togglePersonageTab\"},\"div93\":{\"element\":\"div\",\"style\":{\"position\":\"absolute\",\"top\":\"66px\",\"left\":\"69px\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\",\"width\":\"15px\",\"textAlign\":\"center\"},\"childs\":{\"LVL\":{\"element\":\"strong\",\"text\":\"25\"}}}}},\"div1\":{\"element\":\"div\",\"style\":{\"cssFloat\":\"left\",\"paddingTop\":\"10px\"},\"childs\":{\"HP\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/RedProgress.png\') no-repeat 0 0\",\"backgroundSize\":\"0% 100%\"},\"childs\":{\"HPIMG\":{\"element\":\"img\",\"src\":\"/img/CharProgress.png\"}}},\"MP\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/GreenProgress.png\') no-repeat 0 0\",\"backgroundSize\":\"0% 100%\"},\"childs\":{\"MPIMG\":{\"element\":\"img\",\"src\":\"/img/CharProgress.png\"}}},\"SP\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/BlueProgress.png\') no-repeat 0 0\",\"backgroundSize\":\"0% 100%\"},\"childs\":{\"SPIMG\":{\"element\":\"img\",\"src\":\"/img/CharProgress.png\"}}}}}}},\"div48\":{\"element\":\"div\",\"style\":{\"cssFloat\":\"left\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\",\"paddingTop\":\"7px\",\"paddingLeft\":\"3px\"},\"childs\":{\"HPLABEL\":{\"element\":\"p\",\"style\":{\"margin\":\"5px 0px\"},\"childs\":{\"b92\":{\"element\":\"b\",\"text\":\"HP\"}}},\"MPLABEL\":{\"element\":\"p\",\"style\":{\"margin\":\"5px 0px\"},\"childs\":{\"b19\":{\"element\":\"b\",\"text\":\"MP\"}}},\"SPLABEL\":{\"element\":\"p\",\"style\":{\"margin\":\"5px 0px\"},\"childs\":{\"b36\":{\"element\":\"b\",\"text\":\"SP\"}}}}},\"div14\":{\"element\":\"div\",\"style\":{\"cssFloat\":\"right\",\"padding\":\"20px 5px 0px\"},\"childs\":{\"premiumIcon\":{\"element\":\"img\",\"className\":\"shadow rounded-icon\",\"src\":\"/img/premium.png\",\"onclick\":\"togglePremiumTab\"}}},\"br51\":{\"element\":\"br\",\"style\":{\"clear\":\"both\"}},\"icons\":{\"element\":\"div\",\"style\":{\"textAlign\":\"center\"},\"childs\":{\"inventoryIcon\":{\"element\":\"img\",\"className\":\"shadow rounded-icon\",\"src\":\"/img/iInventory.png\",\"onclick\":\"toggleInventoryTab\"},\"mapIcon\":{\"element\":\"img\",\"className\":\"shadow rounded-icon\",\"src\":\"/img/iMap.png\",\"onclick\":\"toggleMapTab\"},\"shopIcon\":{\"element\":\"img\",\"className\":\"shadow rounded-icon\",\"src\":\"/img/shop.png\",\"onclick\":\"toggleShopTab\"},\"questIcon\":{\"element\":\"img\",\"className\":\"shadow rounded-icon\",\"src\":\"/img/iQuest.png\",\"onclick\":\"toggleQuestTab\"},\"clanIcon\":{\"element\":\"img\",\"className\":\"shadow rounded-icon\",\"src\":\"/img/iClan.png\",\"onclick\":\"toggleClanTab\"},\"duelIcon\":{\"element\":\"img\",\"className\":\"shadow rounded-icon\",\"src\":\"/img/duel.png\",\"onclick\":\"toggleDuelTab\"}}},\"personage-tab\":{\"element\":\"div\",\"className\":\"\",\"style\":{\"textAlign\":\"center\"},\"childs\":{\"div67\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/titles.png\') no-repeat center\",\"padding\":\"10px\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\"},\"childs\":{\"b61\":{\"element\":\"b\",\"text\":\"PERSONAGE\"}}},\"personageImage\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/pers/w.4.png\') no-repeat center\",\"backgroundSize\":\"auto 323px\",\"height\":\"323px\"},\"childs\":{\"headSlot\":{\"element\":\"div\",\"className\":\"cell rounded shadowed gradient\",\"style\":{\"cssFloat\":\"left\"},\"onclick\":\"takeoff\",\"title\":\"Снять\"},\"neckSlot\":{\"element\":\"div\",\"className\":\"cell rounded shadowed gradient\",\"style\":{\"cssFloat\":\"right\"},\"onclick\":\"takeoff\",\"title\":\"Снять\"},\"br13\":{\"element\":\"br\",\"style\":{\"clear\":\"both\"}},\"shouldersSlot\":{\"element\":\"div\",\"className\":\"cell rounded shadowed gradient\",\"style\":{\"cssFloat\":\"left\"},\"onclick\":\"takeoff\",\"title\":\"Снять\"},\"handsSlot\":{\"element\":\"div\",\"className\":\"cell rounded shadowed gradient\",\"style\":{\"cssFloat\":\"right\"},\"onclick\":\"takeoff\",\"title\":\"Снять\"},\"br4\":{\"element\":\"br\",\"style\":{\"clear\":\"both\"}},\"chestSlot\":{\"element\":\"div\",\"className\":\"cell rounded shadowed gradient\",\"style\":{\"cssFloat\":\"left\"},\"onclick\":\"takeoff\",\"title\":\"Снять\"},\"hipSlot\":{\"element\":\"div\",\"className\":\"cell rounded shadowed gradient\",\"style\":{\"cssFloat\":\"right\"},\"onclick\":\"takeoff\",\"title\":\"Снять\"},\"br94\":{\"element\":\"br\",\"style\":{\"clear\":\"both\"}},\"legSlot\":{\"element\":\"div\",\"className\":\"cell rounded shadowed gradient\",\"style\":{\"cssFloat\":\"left\"},\"onclick\":\"takeoff\",\"title\":\"Снять\"},\"footSlot\":{\"element\":\"div\",\"className\":\"cell rounded shadowed gradient\",\"style\":{\"cssFloat\":\"right\"},\"onclick\":\"takeoff\",\"title\":\"Снять\"},\"br2\":{\"element\":\"br\",\"style\":{\"clear\":\"both\"}}}}}},\"inventory-tab\":{\"element\":\"div\",\"className\":\"hidden\",\"style\":{\"textAlign\":\"center\"},\"childs\":{\"div2\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/titles.png\') no-repeat center\",\"padding\":\"10px\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\"},\"childs\":{\"b72\":{\"element\":\"b\",\"text\":\"INVENTORY\"}}},\"inventoryItems\":{\"element\":\"div\",\"style\":{\"padding\":\"2px\",\"height\":\"323px\",\"width\":\"315px\",\"overflowY\":\"scroll\"},\"childs\":{\"div77\":{\"element\":\"div\",\"className\":\"cell\",\"text\":\"Inventory\"}}}}},\"map-tab\":{\"element\":\"div\",\"className\":\"hidden\",\"style\":{\"textAlign\":\"center\"},\"childs\":{\"div47\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/titles.png\') no-repeat center\",\"padding\":\"10px\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\"},\"childs\":{\"b76\":{\"element\":\"b\",\"text\":\"MAP\"}}},\"mapItems\":{\"element\":\"div\",\"style\":{\"padding\":\"2px\",\"height\":\"323px\",\"width\":\"315px\",\"overflowY\":\"scroll\"},\"childs\":{\"mapImage\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/map.png\') no-repeat center\",\"height\":\"800px\",\"width\":\"800px\"}}}}}},\"shop-tab\":{\"element\":\"div\",\"className\":\"hidden\",\"style\":{\"textAlign\":\"center\"},\"childs\":{\"div54\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/titles.png\') no-repeat center\",\"padding\":\"10px\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\"},\"childs\":{\"b22\":{\"element\":\"b\",\"text\":\"SHOP\"}}},\"shopItems\":{\"element\":\"div\",\"style\":{\"padding\":\"2px\",\"height\":\"323px\",\"width\":\"315px\",\"overflowY\":\"scroll\"},\"childs\":{\"div67\":{\"element\":\"div\",\"className\":\"cell\",\"text\":\"Shop\"}}}}},\"quest-tab\":{\"element\":\"div\",\"className\":\"hidden\",\"style\":{\"textAlign\":\"center\"},\"childs\":{\"div67\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/titles.png\') no-repeat center\",\"padding\":\"10px\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\"},\"childs\":{\"b36\":{\"element\":\"b\",\"text\":\"QUEST\"}}},\"questItems\":{\"element\":\"div\",\"style\":{\"padding\":\"2px\",\"height\":\"323px\",\"width\":\"315px\",\"overflowY\":\"scroll\"},\"childs\":{\"div21\":{\"element\":\"div\",\"className\":\"cell\",\"text\":\"Quest\"}}}}},\"clan-tab\":{\"element\":\"div\",\"className\":\"hidden\",\"style\":{\"textAlign\":\"center\"},\"childs\":{\"div77\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/titles.png\') no-repeat center\",\"padding\":\"10px\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\"},\"childs\":{\"b25\":{\"element\":\"b\",\"text\":\"CLAN\"}}},\"clanItems\":{\"element\":\"div\",\"style\":{\"padding\":\"2px\",\"height\":\"323px\",\"width\":\"315px\",\"overflowY\":\"scroll\"},\"childs\":{\"div18\":{\"element\":\"div\",\"className\":\"cell\",\"text\":\"Clan\"}}}}},\"duel-tab\":{\"element\":\"div\",\"className\":\"hidden\",\"style\":{\"textAlign\":\"center\"},\"childs\":{\"div60\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/titles.png\') no-repeat center\",\"padding\":\"10px\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\"},\"childs\":{\"b96\":{\"element\":\"b\",\"text\":\"DUEL\"}}},\"attackUser\":{\"element\":\"div\",\"style\":{\"position\":\"relative\",\"width\":\"315px\",\"cssFloat\":\"right\",\"textAlign\":\"center\"},\"childs\":{\"h237\":{\"element\":\"h2\",\"text\":\"Attack?\"},\"userToAttack\":{\"element\":\"span\",\"className\":\"label important\",\"text\":\"asd\"},\"br54\":{\"element\":\"br\"},\"challengeDuel\":{\"element\":\"a\",\"href\":\"#\",\"className\":\"btn warning\",\"onclick\":\"challenge\",\"text\":\"Challenge\"},\"acceptChallenge\":{\"element\":\"a\",\"href\":\"#\",\"className\":\"btn warning\",\"onclick\":\"acceptChallenge\",\"text\":\"Fight\"},\"br6\":{\"element\":\"br\"},\"rejectChallenge\":{\"element\":\"a\",\"href\":\"#\",\"className\":\"btn warning\",\"onclick\":\"rejectChallenge\",\"text\":\"Run Away\"}}}}},\"premium-tab\":{\"element\":\"div\",\"className\":\"hidden\",\"style\":{\"textAlign\":\"center\"},\"childs\":{\"div54\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/titles.png\') no-repeat center\",\"padding\":\"10px\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\"},\"childs\":{\"b47\":{\"element\":\"b\",\"text\":\"PREMIUM\"}}},\"premiumItems\":{\"element\":\"div\",\"style\":{\"padding\":\"2px\",\"height\":\"323px\",\"width\":\"315px\",\"overflowY\":\"scroll\"},\"childs\":{\"div25\":{\"element\":\"div\",\"className\":\"cell\",\"text\":\"Buy premium\"}}}}}}}}'),(11,'map-window','{\"map-window\":{\"element\":\"div\",\"className\":\"rounded shadowed\",\"style\":{\"position\":\"fixed\",\"display\":\"none\",\"background\":\"url(\'/img/map.png\') repeat scroll center center rgb(255, 255, 255)\",\"top\":\"0px\",\"right\":\"245px\",\"left\":\"980px\",\"padding\":\"10px\",\"minHeight\":\"148px\"},\"text\":\"Map\"}}'),(12,'premium-window','{\"premium-window\":{\"element\":\"div\",\"className\":\"rounded shadowed\",\"style\":{\"position\":\"fixed\",\"width\":\"220px\",\"left\":\"245px\",\"top\":\"0%\",\"display\":\"none\",\"background\":\"none repeat scroll 0% 0% rgb(255, 255, 255)\",\"padding\":\"10px\",\"minHeight\":\"148px\"},\"text\":\"Premium\",\"childs\":{\"mapimg\":{\"element\":\"img\",\"src\":\"/img/premium.png\"}}}}'),(13,'avatar-window','{\"avatar-window\":{\"element\":\"div\",\"className\":\"rounded shadowed\",\"style\":{\"position\":\"fixed\",\"width\":\"220px\",\"left\":\"0%\",\"top\":\"173px\",\"display\":\"none\",\"background\":\"none repeat scroll 0% 0% rgb(255, 255, 255)\",\"padding\":\"10px\",\"minHeight\":\"148px\"},\"childs\":{\"avatarIcon\":{\"element\":\"img\",\"src\":\"/img/boy.png\",\"onclick\":\"toggleShop\"},\"personage_stats\":{\"element\":\"table\",\"style\":{\"border\":\"medium none\",\"padding\":\"0px\"}}}}}'),(14,'register-window','{\"register-window\":{\"element\":\"div\",\"className\":\"bordered rounded shadowed hidden\",\"style\":{\"position\":\"absolute\",\"width\":\"220px\",\"left\":\"50%\",\"top\":\"50%\",\"marginLeft\":\"-110px\",\"padding\":\"10px\"},\"childs\":{\"div89\":{\"element\":\"div\",\"style\":{\"background\":\"url(\'/img/titles.png\') no-repeat center\",\"padding\":\"10px\",\"color\":\"white\",\"textShadow\":\"black 1px 1px 2px, red 0 0 1em\",\"textAlign\":\"center\"},\"childs\":{\"b14\":{\"element\":\"b\",\"text\":\"REGISTRATION\"}}},\"img15\":{\"element\":\"img\",\"className\":\"bgimage\",\"src\":\"/img/bg.jpg\"},\"llogin\":{\"element\":\"span\",\"style\":{\"display\":\"block\"},\"text\":\"Login\"},\"register_login\":{\"element\":\"input\",\"className\":\"span4\"},\"lpassword\":{\"element\":\"span\",\"style\":{\"display\":\"block\"},\"text\":\"Password\"},\"register_password\":{\"element\":\"input\",\"className\":\"span4\",\"style\":{\"display\":\"block\",\"marginBottom\":\"10px\"}},\"cancelBtn\":{\"element\":\"button\",\"className\":\"btn\",\"style\":{\"width\":\"104px\",\"marginRight\":\"5px\"},\"onclick\":\"showLogin\",\"text\":\"Cancel\"},\"registerBtn\":{\"element\":\"button\",\"className\":\"btn primary\",\"style\":{\"width\":\"104px\",\"marginLeft\":\"5px\"},\"onclick\":\"register\",\"text\":\"Register\"}}}}');
/*!40000 ALTER TABLE `client_forms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `client_methods`
--

DROP TABLE IF EXISTS `client_methods`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_methods` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Method Name',
  `body` text NOT NULL COMMENT 'Method Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `client_methods`
--

LOCK TABLES `client_methods` WRITE;
/*!40000 ALTER TABLE `client_methods` DISABLE KEYS */;
INSERT INTO `client_methods` VALUES (1,'carousel','function carousel() {\n	if (context)\n	{\n		if (!window.angle) window.angle = 0;\n		window.angle +=0.05;\n		context.save();\n		// clear the drawing surface\n		context.clearRect(0, 0, canvas.width, canvas.height);\n		\n		var delta = Math.PI/personages.length*2;\n		for ( var idx = 0; idx < personages.length; idx++) {\n			personages[idx].angle = idx+angle; \n			personages[idx].flag = Math.sin(delta*(idx+angle));\n		}\n		var newArray = personages.slice();\n		function sortfunction(a, b){\n		//Compare \"a\" and \"b\" in some fashion, and return -1, 0, or 1\n			return a.flag - b.flag;\n		}\n		newArray.sort(sortfunction);\n		for ( var idx = 0; idx < newArray.length; idx++) {\n			var obj = newArray[idx];\n			var sx = 1;\n			if (idx == newArray.length-1)\n				setShadow(context, \"#FFFF88\", 0, 0, 15);\n				obj.draw(context, Math.cos(delta*(obj.angle))*500+canvas.width/2-100, -Math.sin(delta*(obj.angle))*20+200, obj.scaleFactor*0.4+Math.sin(delta*(obj.angle))/10);\n			if (idx == newArray.length -1)\n				clearShadow(context);\n		}\n		context.setTransform(1, 0, 0, 1, 0, 1);\n		context.restore();\n	}\n}'),(2,'clearCanvas','function clearCanvas(data) {\n	context.save();\n	// clear the drawing surface\n	context.clearRect(0, 0, canvas.width, canvas.height);\n	context.setTransform(1, 0, 0, 1, 0, 1);\n	context.restore();\n};'),(3,'hideWindow','function hideWindow(data) {\n	//ui.getByName(data.params).style.display = \'none\';\n	document.getElementById(data.params).style.display = \'none\';\n};'),(4,'login','function login(data) {\n		window.user = $(\'#user_login\').val();\n		resp.push({\"cmd\" : \"login\", \"login\" : $(\'#user_login\').val(), \"password\" : $(\'#user_password\').val()});\n		jsonRPC({\n			\"method\" : \"ad.serve\",\n			\"params\" : resp,\n			\"id\" : \"id value (optional)\"\n		}, onData, onError);\n		return false;\n	};'),(5,'register','function register(data) {\n		resp.push({\"cmd\" : \"register\", \"login\" : $(\'#register_login\').val(), \"password\" : $(\'#register_password\').val()});\n		jsonRPC({\n			\"method\" : \"ad.serve\",\n			\"params\" : resp,\n			\"id\" : \"id value (optional)\"\n		}, onData, onError);\n		return false;\n	};'),(6,'showLogin','function showLogin(data) {\n	ui.getByName(\'login-window\').style.display = \'block\';\n	ui.getByName(\'register-window\').style.display = \'none\';\n};'),(7,'showModal','function showModal(data) {\n	$(\'#modal-header h3:first\').text(data.params.header);\n	$(\'#modal-body\').html(data.params.body);\n	$(\'#modal\').modal({\n		\'show\' : true,\n		\'keyboard\' : true,\n		\'backdrop\' : true\n	});\n};'),(8,'showRegister','function showRegister(data) {\n	ui.getByName(\'login-window\').style.display = \'none\';\n	ui.getByName(\'register-window\').style.display = \'block\';\n};'),(9,'showWindow','function showWindow(data) {\n	//ui.getByName(data.params).style.display = \'block\';\n	document.getElementById(data.params).style.display = \'block\';\n};'),(10,'startCarousel','function startCarousel(data) {\n	window.carouselId = setInterval(methods[\'carousel\'], 80);\n};'),(11,'stopCarousel','function stopCarousel(data) {\n	clearInterval(carouselId);\n};'),(12,'alert','function method(data) {\n        alert(data.params);\n}');
/*!40000 ALTER TABLE `client_methods` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `client_style`
--

DROP TABLE IF EXISTS `client_style`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_style` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Class Name',
  `body` text NOT NULL COMMENT 'Class Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `client_style`
--

LOCK TABLES `client_style` WRITE;
/*!40000 ALTER TABLE `client_style` DISABLE KEYS */;
INSERT INTO `client_style` VALUES (1,'asd','zxc');
/*!40000 ALTER TABLE `client_style` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `context`
--

DROP TABLE IF EXISTS `context`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `context` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type_id` int(11) NOT NULL DEFAULT '0',
  `title` varchar(255) NOT NULL DEFAULT '',
  `user_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `context`
--

LOCK TABLES `context` WRITE;
/*!40000 ALTER TABLE `context` DISABLE KEYS */;
/*!40000 ALTER TABLE `context` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `context_groups`
--

DROP TABLE IF EXISTS `context_groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `context_groups` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(64) NOT NULL DEFAULT '',
  `description` mediumtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `context_groups`
--

LOCK TABLES `context_groups` WRITE;
/*!40000 ALTER TABLE `context_groups` DISABLE KEYS */;
/*!40000 ALTER TABLE `context_groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `context_types`
--

DROP TABLE IF EXISTS `context_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `context_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `group_id` int(11) NOT NULL DEFAULT '0',
  `title` varchar(64) NOT NULL DEFAULT '',
  `description` mediumtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `context_types`
--

LOCK TABLES `context_types` WRITE;
/*!40000 ALTER TABLE `context_types` DISABLE KEYS */;
/*!40000 ALTER TABLE `context_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `events`
--

DROP TABLE IF EXISTS `events`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `events` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Event Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Event Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Game Events';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `events`
--

LOCK TABLES `events` WRITE;
/*!40000 ALTER TABLE `events` DISABLE KEYS */;
INSERT INTO `events` VALUES (1,'asd','zxc');
/*!40000 ALTER TABLE `events` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `game_log`
--

DROP TABLE IF EXISTS `game_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `game_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Game Log';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `game_log`
--

LOCK TABLES `game_log` WRITE;
/*!40000 ALTER TABLE `game_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `game_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `graphics`
--

DROP TABLE IF EXISTS `graphics`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `graphics` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Graphics Item Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Graphics Item Body',
  `type` int(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Game Events';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `graphics`
--

LOCK TABLES `graphics` WRITE;
/*!40000 ALTER TABLE `graphics` DISABLE KEYS */;
INSERT INTO `graphics` VALUES (1,'qwe','asdsdfgdgfg',1);
/*!40000 ALTER TABLE `graphics` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `graphics_types`
--

DROP TABLE IF EXISTS `graphics_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `graphics_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Location Type Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Location Type Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COMMENT='Personage Types';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `graphics_types`
--

LOCK TABLES `graphics_types` WRITE;
/*!40000 ALTER TABLE `graphics_types` DISABLE KEYS */;
INSERT INTO `graphics_types` VALUES (1,'tileset','Tile Set'),(2,'autoset','Auto Set');
/*!40000 ALTER TABLE `graphics_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `guilds`
--

DROP TABLE IF EXISTS `guilds`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `guilds` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Guild Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Guild Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Guilds Data';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `guilds`
--

LOCK TABLES `guilds` WRITE;
/*!40000 ALTER TABLE `guilds` DISABLE KEYS */;
INSERT INTO `guilds` VALUES (1,'qaweq','asdf');
/*!40000 ALTER TABLE `guilds` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `location_types`
--

DROP TABLE IF EXISTS `location_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `location_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Location Type Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Location Type Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Personage Types';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `location_types`
--

LOCK TABLES `location_types` WRITE;
/*!40000 ALTER TABLE `location_types` DISABLE KEYS */;
INSERT INTO `location_types` VALUES (1,'map','Карта');
/*!40000 ALTER TABLE `location_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `locations`
--

DROP TABLE IF EXISTS `locations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locations` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Location Title',
  `body` text NOT NULL,
  `type` int(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Locataions Data';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `locations`
--

LOCK TABLES `locations` WRITE;
/*!40000 ALTER TABLE `locations` DISABLE KEYS */;
INSERT INTO `locations` VALUES (1,'Base','{\"map\":[[[48,null,null],[48,null,null],[52,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[86,null,null],[407,null,null],[415,null,null],[385,null,null],[69,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[72,null,null],[50,null,null]],[[48,null,null],[48,null,null],[76,null,null],[405,null,null],[413,null,null],[521,null,506],[520,437,514],[520,437,null],[385,456,null],[385,472,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[81,null,null],[405,null,null],[413,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[76,null,null],[406,535,null],[414,543,null],[385,null,null],[385,null,null],[385,null,null],[385,458,null],[385,474,null],[385,null,null],[385,null,null],[83,null,null],[80,null,null],[87,null,null],[407,null,null],[415,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[76,null,null],[406,null,547],[414,null,555],[385,null,null],[83,null,null],[80,null,null],[80,null,null],[80,null,null],[80,null,null],[80,null,null],[87,null,null],[407,null,null],[415,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[76,null,null],[406,477,548],[414,null,556],[393,null,null],[81,null,null],[405,null,null],[413,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[76,null,null],[406,null,549],[414,null,557],[393,null,null],[81,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[76,null,null],[406,468,null],[414,null,null],[393,null,null],[81,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[52,null,null],[86,null,null],[407,null,null],[415,null,null],[393,null,null],[81,null,null],[407,null,null],[415,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[52,null,null],[86,null,null],[405,null,null],[413,534,null],[385,542,null],[393,null,null],[70,null,null],[64,null,null],[64,null,null],[66,null,null],[80,null,null],[80,null,null],[80,null,null],[80,null,null],[89,null,null],[405,null,null],[413,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,534,null],[385,542,551],[385,null,null],[385,null,null],[68,null,null],[48,null,null],[48,null,null],[76,null,null],[405,null,null],[413,416,null],[385,432,null],[385,null,null],[81,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[56,null,null],[88,null,null],[407,null,null],[415,534,null],[385,542,null],[393,null,null],[84,null,null],[50,null,null],[48,null,null],[76,null,null],[406,null,null],[414,417,476],[385,433,438],[385,null,null],[81,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[56,null,null],[88,null,null],[405,null,null],[413,null,null],[393,null,null],[393,null,null],[68,null,null],[48,null,null],[76,null,null],[406,null,null],[414,418,null],[385,434,null],[385,null,null],[81,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[76,null,null],[406,468,null],[414,null,null],[393,null,null],[393,null,null],[84,null,null],[72,null,null],[86,null,null],[407,null,535],[415,489,543],[385,497,null],[385,null,null],[81,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[76,null,null],[406,null,null],[414,null,null],[393,null,null],[393,532,null],[385,540,533],[385,null,541],[385,456,null],[385,464,null],[385,472,null],[385,null,null],[385,null,null],[81,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[76,null,null],[406,427,null],[414,null,null],[393,538,null],[385,null,null],[385,null,null],[385,null,null],[385,457,null],[385,465,null],[385,473,null],[385,null,null],[385,null,null],[93,null,null],[407,null,null],[415,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[76,null,null],[406,419,null],[414,null,null],[385,539,null],[385,null,null],[385,null,null],[385,null,null],[385,458,null],[385,466,null],[384,474,null],[384,null,null],[384,null,null],[384,null,null],[384,null,null],[384,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[76,null,null],[406,null,null],[414,null,null],[385,528,null],[385,536,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[384,null,null],[393,null,null],[393,null,null],[393,null,null],[393,null,null],[384,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[76,null,null],[407,null,null],[415,null,null],[385,529,null],[385,537,null],[385,null,null],[385,533,null],[385,541,null],[385,null,null],[384,null,null],[393,null,null],[393,null,null],[91,null,null],[405,null,null],[413,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[48,null,null],[48,null,null],[56,null,null],[64,null,null],[88,null,null],[405,null,null],[413,null,null],[385,530,null],[82,null,null],[88,null,null],[405,null,null],[413,null,532],[393,540,533],[393,null,541],[81,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[52,null,null],[72,null,null],[72,null,null],[72,null,null],[73,null,null],[80,null,null],[80,null,null],[80,null,null],[74,null,null],[73,null,null],[80,null,null],[80,null,null],[80,null,null],[80,null,null],[87,null,null],[407,null,null],[415,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[405,null,null],[413,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[406,null,null],[414,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[76,null,null],[407,null,null],[415,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[385,null,null],[68,null,null]],[[56,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[64,null,null],[49,null,null]]],\"propreties\":{\"48\":[5,31],\"52\":[5,31],\"72\":[5,31],\"86\":[5,31],\"407\":[0,15],\"415\":[0,15],\"385\":[0,0],\"68\":[5,16],\"76\":[5,31],\"405\":[0,15],\"413\":[0,15],\"521\":[0,15],\"506\":[1,1],\"520\":[0,15],\"437\":[0,0],\"514\":[0,8],\"456\":[0,0],\"472\":[0,0],\"406\":[0,15],\"535\":[0,15],\"414\":[0,15],\"543\":[0,15],\"458\":[0,0],\"474\":[0,0],\"82\":[5,16],\"64\":[5,31],\"49\":[5,31],\"547\":[0,15],\"555\":[0,15],\"477\":[0,0],\"548\":[0,15],\"556\":[0,15],\"393\":[0,0],\"549\":[0,15],\"557\":[0,15],\"468\":[0,15],\"534\":[1,0],\"542\":[0,15],\"50\":[5,31],\"551\":[0,15],\"416\":[1,1],\"432\":[0,8],\"56\":[5,31],\"88\":[5,31],\"84\":[5,16],\"417\":[1,1],\"476\":[1,0],\"433\":[0,8],\"438\":[0,0],\"418\":[1,1],\"434\":[0,8],\"489\":[1,1],\"497\":[0,8],\"532\":[1,0],\"540\":[0,15],\"533\":[1,0],\"541\":[0,15],\"464\":[0,0],\"427\":[0,15],\"538\":[0,15],\"457\":[0,0],\"465\":[0,0],\"473\":[0,0],\"419\":[0,15],\"539\":[0,15],\"466\":[0,0],\"384\":[0,0],\"528\":[0,15],\"536\":[0,15],\"529\":[0,15],\"537\":[0,15],\"530\":[0,15]}}',1);
/*!40000 ALTER TABLE `locations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `logic`
--

DROP TABLE IF EXISTS `logic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `logic` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `object` text NOT NULL COMMENT 'Logic Object',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Modification Time',
  `modified_by` int(11) NOT NULL DEFAULT '0' COMMENT 'User Who Modified Logic',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=16 DEFAULT CHARSET=utf8 COMMENT='Quest Scripts';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `logic`
--

LOCK TABLES `logic` WRITE;
/*!40000 ALTER TABLE `logic` DISABLE KEYS */;
INSERT INTO `logic` VALUES (1,'{}','2012-01-11 08:21:43',0),(2,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начальный квест\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"New Trigger\",\"x\":728.0023192493529,\"y\":162.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"or\":\"1340cb73-ea40-4c77-b4d2-da5eda07828f\",\"conditions\":[]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"New Instruction\",\"x\":871.7418873637989,\"y\":297.24604203787476,\"conditions\":[],\"steps\":[]},\"1340cb73-ea40-4c77-b4d2-da5eda07828f\":{\"type\":\"INSTRUCTION\",\"text\":\"New Instruction\",\"x\":627.7024437321172,\"y\":326.2546369191067,\"conditions\":[],\"steps\":[]}}}}','2012-01-11 08:22:28',0),(3,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начальный квест\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Начальный триггер\",\"x\":728.0023192493529,\"y\":162.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"or\":\"1340cb73-ea40-4c77-b4d2-da5eda07828f\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Положительно\",\"x\":871.7418873637989,\"y\":297.24604203787476,\"conditions\":[],\"steps\":[]},\"1340cb73-ea40-4c77-b4d2-da5eda07828f\":{\"type\":\"INSTRUCTION\",\"text\":\"Oтрицательно\",\"x\":627.7024437321172,\"y\":326.2546369191067,\"conditions\":[],\"steps\":[]}}}}','2012-01-11 09:03:24',0),(4,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начальный квест\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Начальный триггер\",\"x\":731.0023192493529,\"y\":156.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"or\":\"1340cb73-ea40-4c77-b4d2-da5eda07828f\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Положительно\",\"x\":953.7418873637989,\"y\":338.24604203787476,\"conditions\":[],\"steps\":[]},\"1340cb73-ea40-4c77-b4d2-da5eda07828f\":{\"type\":\"INSTRUCTION\",\"text\":\"Oтрицательно\",\"x\":627.7024437321172,\"y\":326.2546369191067,\"conditions\":[],\"steps\":[]}}}}','2012-01-11 09:07:12',0),(5,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начальный квест\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Начальный триггер\",\"x\":731.0023192493529,\"y\":156.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"or\":\"1340cb73-ea40-4c77-b4d2-da5eda07828f\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Положительно\",\"x\":953.7418873637989,\"y\":338.24604203787476,\"conditions\":[],\"steps\":[]},\"1340cb73-ea40-4c77-b4d2-da5eda07828f\":{\"type\":\"INSTRUCTION\",\"text\":\"Oтрицательно\",\"x\":627.7024437321172,\"y\":326.2546369191067,\"conditions\":[],\"steps\":[]}}}}','2012-01-11 09:07:37',0),(6,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начальный квест\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Начальный триггер\",\"x\":680.0023192493529,\"y\":86.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"or\":\"1340cb73-ea40-4c77-b4d2-da5eda07828f\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Положительно\",\"x\":922.7418873637989,\"y\":273.24604203787476,\"conditions\":[],\"steps\":[]},\"1340cb73-ea40-4c77-b4d2-da5eda07828f\":{\"type\":\"INSTRUCTION\",\"text\":\"Oтрицательно\",\"x\":488.7024437321172,\"y\":361.2546369191067,\"conditions\":[],\"steps\":[]}}}}','2012-01-11 09:20:12',0),(7,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начало игры\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Не выбрана расса, пол и тип персонажа\",\"x\":364.0023192493529,\"y\":93.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"or\":\"1340cb73-ea40-4c77-b4d2-da5eda07828f\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Положительно\",\"x\":741.7418873637989,\"y\":262.24604203787476,\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"SELECT_RACE\",\"client_param\":\"\"}]}}}}','2012-01-30 09:24:59',0),(8,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начало игры\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Не выбрана расса, пол и тип персонажа\",\"x\":364.0023192493529,\"y\":93.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"or\":\"1340cb73-ea40-4c77-b4d2-da5eda07828f\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Положительно\",\"x\":741.7418873637989,\"y\":262.24604203787476,\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"SELECT_RACE\",\"client_param\":\"\"}]}}}}','2012-01-30 09:25:00',0),(9,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начало игры\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Не выбрана расса, пол и тип персонажа\",\"x\":472.0023192493529,\"y\":83.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Положительно\",\"x\":842.7418873637989,\"y\":279.24604203787476,\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"selectRace\",\"client_param\":\"\"}]}}}}','2012-01-31 05:58:57',0),(10,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начало игры\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Не выбрана расса, пол и тип персонажа\",\"x\":472.0023192493529,\"y\":83.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Положительно\",\"x\":842.7418873637989,\"y\":279.24604203787476,\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"startSelectRace\",\"client_param\":\"\"}]}}}}','2012-01-31 06:42:15',0),(11,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начало игры\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Не выбрана расса, пол и тип персонажа\",\"x\":472.0023192493529,\"y\":83.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Выбираем рассу\",\"x\":842.7418873637989,\"y\":279.24604203787476,\"next\":\"52e07c9b-67f8-492b-83cd-194ed06d725d\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"startSelectRace\",\"client_param\":\"\"}]},\"52e07c9b-67f8-492b-83cd-194ed06d725d\":{\"type\":\"INSTRUCTION\",\"text\":\"Устанавливаем уровень\",\"x\":860.3872252471103,\"y\":408.99657471671424,\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"race\",\"condition\":\"!=\",\"val\":\"0\"}],\"steps\":[{\"object\":\"personage\",\"cmd\":\"SET\",\"param\":\"lvl\",\"val\":\"1\",\"client_cmd\":\"\",\"client_param\":\"\"}]}}}}','2012-01-31 10:12:39',0),(12,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начало игры\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Не выбрана расса, пол и тип персонажа\",\"x\":472.0023192493529,\"y\":83.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"or\":\"f366c92e-d0e9-440f-82d7-44efea90689d\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Выбираем рассу\",\"x\":842.7418873637989,\"y\":279.24604203787476,\"next\":\"52e07c9b-67f8-492b-83cd-194ed06d725d\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"startSelectRace\",\"client_param\":\"\"}]},\"52e07c9b-67f8-492b-83cd-194ed06d725d\":{\"type\":\"INSTRUCTION\",\"text\":\"Устанавливаем уровень\",\"x\":860.3872252471103,\"y\":408.99657471671424,\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"race\",\"condition\":\"!=\",\"val\":\"0\"}],\"steps\":[{\"object\":\"personage\",\"cmd\":\"SET\",\"param\":\"lvl\",\"val\":\"1\",\"client_cmd\":\"\",\"client_param\":\"\"}]},\"f366c92e-d0e9-440f-82d7-44efea90689d\":{\"type\":\"INSTRUCTION\",\"text\":\"Не сработало\",\"x\":432.4699008280272,\"y\":280.5132255325717,\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"alert\",\"client_param\":\"Вы уже здесь были\"}]}}}}','2012-01-31 11:07:34',0),(13,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начало игры\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Не выбрана расса, пол и тип персонажа\",\"x\":472.0023192493529,\"y\":83.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"or\":\"f366c92e-d0e9-440f-82d7-44efea90689d\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Выбираем рассу\",\"x\":842.7418873637989,\"y\":279.24604203787476,\"next\":\"52e07c9b-67f8-492b-83cd-194ed06d725d\",\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"startSelectRace\",\"client_param\":\"\"}]},\"52e07c9b-67f8-492b-83cd-194ed06d725d\":{\"type\":\"INSTRUCTION\",\"text\":\"Устанавливаем уровень\",\"x\":860.3872252471103,\"y\":408.99657471671424,\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"SET\",\"param\":\"lvl\",\"val\":\"1\",\"client_cmd\":\"\",\"client_param\":\"\"}]},\"f366c92e-d0e9-440f-82d7-44efea90689d\":{\"type\":\"INSTRUCTION\",\"text\":\"Не сработало\",\"x\":432.4699008280272,\"y\":280.5132255325717,\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"alert\",\"client_param\":\"Вы уже здесь были\"}]}}}}','2012-02-07 09:24:21',0),(14,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начало игры\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Не выбрана расса, пол и тип персонажа\",\"x\":472.0023192493529,\"y\":83.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"or\":\"f366c92e-d0e9-440f-82d7-44efea90689d\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"aa93a737-929b-4722-8024-d4658df42d81\":{\"type\":\"TRIGGER\",\"text\":\"New Trigger\",\"x\":820.4384349913463,\"y\":283.24483511886973,\"next\":\"52e07c9b-67f8-492b-83cd-194ed06d725d\",\"or\":\"b5084324-b6a5-462c-8eed-780e9e18f1fd\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Выбираем рассу\",\"x\":751.7418873637989,\"y\":187.24604203787476,\"next\":\"aa93a737-929b-4722-8024-d4658df42d81\",\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"startSelectRace\",\"client_param\":\"\"},{\"object\":\"\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"\",\"client_param\":\"\"}]},\"52e07c9b-67f8-492b-83cd-194ed06d725d\":{\"type\":\"INSTRUCTION\",\"text\":\"Устанавливаем уровень\",\"x\":914.3872252471103,\"y\":408.99657471671424,\"next\":\"aa93a737-929b-4722-8024-d4658df42d81\",\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"SET\",\"param\":\"lvl\",\"val\":\"1\",\"client_cmd\":\"\",\"client_param\":\"\"}]},\"f366c92e-d0e9-440f-82d7-44efea90689d\":{\"type\":\"INSTRUCTION\",\"text\":\"Не сработало\",\"x\":433.4699008280272,\"y\":278.5132255325717,\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"alert\",\"client_param\":\"Вы уже здесь были\"}]},\"b5084324-b6a5-462c-8eed-780e9e18f1fd\":{\"type\":\"INSTRUCTION\",\"text\":\"New Instruction\",\"x\":696.3836974909466,\"y\":396.60848692188404,\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"alert\",\"client_param\":\"Вы уже здесь были\"}]}}}}','2012-02-07 10:18:31',0),(15,'{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"text\":\"Начало игры\",\"type\":\"STATE\",\"logics\":{\"ec18a39c-f0e3-4129-b72b-447184a79fd8\":{\"type\":\"TRIGGER\",\"text\":\"Не выбрана расса, пол и тип персонажа\",\"x\":472.0023192493529,\"y\":83.32933838925288,\"next\":\"ab740ff3-1dea-41b7-903c-889ddf65c95a\",\"or\":\"f366c92e-d0e9-440f-82d7-44efea90689d\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"aa93a737-929b-4722-8024-d4658df42d81\":{\"type\":\"TRIGGER\",\"text\":\"New Trigger\",\"x\":860.4384349913463,\"y\":378.24483511886973,\"next\":\"52e07c9b-67f8-492b-83cd-194ed06d725d\",\"or\":\"b5084324-b6a5-462c-8eed-780e9e18f1fd\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"lvl\",\"condition\":\"==\",\"val\":\"0\"}]},\"ba409f75-cfff-4d6d-9e7f-7f2c1f1d4727\":{\"type\":\"TRIGGER\",\"text\":\"Ожилаем выбор рассы\",\"x\":736.5505736745448,\"y\":272.923985315206,\"next\":\"aa93a737-929b-4722-8024-d4658df42d81\",\"conditions\":[{\"object\":\"personage\",\"cmd\":\"SELECT\",\"param\":\"race\",\"condition\":\"!=\",\"val\":\"0\"}]},\"ab740ff3-1dea-41b7-903c-889ddf65c95a\":{\"type\":\"INSTRUCTION\",\"text\":\"Выбираем рассу\",\"x\":725.7418873637989,\"y\":173.24604203787476,\"next\":\"ba409f75-cfff-4d6d-9e7f-7f2c1f1d4727\",\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"startSelectRace\",\"client_param\":\"\"}]},\"52e07c9b-67f8-492b-83cd-194ed06d725d\":{\"type\":\"INSTRUCTION\",\"text\":\"Устанавливаем уровень\",\"x\":1021.3872252471103,\"y\":476.99657471671424,\"next\":\"aa93a737-929b-4722-8024-d4658df42d81\",\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"SET\",\"param\":\"lvl\",\"val\":\"1\",\"client_cmd\":\"\",\"client_param\":\"\"}]},\"f366c92e-d0e9-440f-82d7-44efea90689d\":{\"type\":\"INSTRUCTION\",\"text\":\"Не сработало\",\"x\":433.4699008280272,\"y\":278.5132255325717,\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"alert\",\"client_param\":\"Вы уже здесь были\"}]},\"b5084324-b6a5-462c-8eed-780e9e18f1fd\":{\"type\":\"INSTRUCTION\",\"text\":\"New Instruction\",\"x\":712.3836974909466,\"y\":459.60848692188404,\"conditions\":[],\"steps\":[{\"object\":\"personage\",\"cmd\":\"\",\"param\":\"\",\"val\":\"\",\"client_cmd\":\"alert\",\"client_param\":\"Вы уже здесь были\"}]}}}}','2012-02-07 10:36:30',0);
/*!40000 ALTER TABLE `logic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mails`
--

DROP TABLE IF EXISTS `mails`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mails` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Mail Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Mail Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Game Mails';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mails`
--

LOCK TABLES `mails` WRITE;
/*!40000 ALTER TABLE `mails` DISABLE KEYS */;
INSERT INTO `mails` VALUES (1,'qwe','qwe');
/*!40000 ALTER TABLE `mails` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `messages`
--

DROP TABLE IF EXISTS `messages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `messages` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Chat Messages';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `messages`
--

LOCK TABLES `messages` WRITE;
/*!40000 ALTER TABLE `messages` DISABLE KEYS */;
/*!40000 ALTER TABLE `messages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `news`
--

DROP TABLE IF EXISTS `news`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `news` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'News Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'News Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Administrative News';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `news`
--

LOCK TABLES `news` WRITE;
/*!40000 ALTER TABLE `news` DISABLE KEYS */;
INSERT INTO `news` VALUES (1,'qwe','qwe');
/*!40000 ALTER TABLE `news` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `npc`
--

DROP TABLE IF EXISTS `npc`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `npc` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'NPC Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'NPC Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='NPC Data By Types';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `npc`
--

LOCK TABLES `npc` WRITE;
/*!40000 ALTER TABLE `npc` DISABLE KEYS */;
INSERT INTO `npc` VALUES (1,'qwe','qwe');
/*!40000 ALTER TABLE `npc` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `personage`
--

DROP TABLE IF EXISTS `personage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `personage` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL DEFAULT '0',
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Personage Title',
  `body` text NOT NULL COMMENT 'Personage Context',
  `role` int(1) NOT NULL DEFAULT '0' COMMENT 'Angels/Daemons',
  `type` int(1) NOT NULL DEFAULT '0' COMMENT 'Rebel/Learner/Adventurer etc',
  `sex` int(1) NOT NULL DEFAULT '0' COMMENT 'Male/Female',
  `hp` decimal(5,2) NOT NULL DEFAULT '0.00' COMMENT 'Health Power',
  `ap` decimal(5,2) NOT NULL DEFAULT '0.00' COMMENT 'Musclule mass (Attack Power)',
  `dex` decimal(5,2) NOT NULL DEFAULT '0.00' COMMENT 'Dexterity',
  `iq` decimal(5,2) NOT NULL DEFAULT '0.00' COMMENT 'IQ',
  `ss` decimal(5,2) NOT NULL DEFAULT '0.00' COMMENT 'Sixth Sense',
  `luck` decimal(5,2) NOT NULL DEFAULT '0.00' COMMENT 'Luck/Drag/Communications',
  `sm` decimal(5,2) NOT NULL DEFAULT '0.00' COMMENT 'Strength Of Mind',
  `money` decimal(5,2) NOT NULL DEFAULT '0.00' COMMENT 'Money',
  `gold` decimal(5,2) NOT NULL DEFAULT '0.00' COMMENT 'Gold',
  `mana` decimal(5,2) NOT NULL DEFAULT '0.00' COMMENT 'Mana',
  `adamatin` int(11) NOT NULL DEFAULT '0' COMMENT 'Adamatin',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=utf8 COMMENT='Personage Characteristics';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `personage`
--

LOCK TABLES `personage` WRITE;
/*!40000 ALTER TABLE `personage` DISABLE KEYS */;
INSERT INTO `personage` VALUES (1,0,'qwe','qwe',0,0,0,'0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00',0),(2,2,'','{\"tasks\":{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"title\":\"Начало игры\",\"instruction\":\"b5084324-b6a5-462c-8eed-780e9e18f1fd\",\"closed\":true}},\"personage\":{\"lvl\":\"1\",\"race\":\"daemons\"},\"personageId\":2,\"subscription\":{\"0\":\"Main Channel\"},\"messages\":[{\"authid\":2,\"author\":\"test\",\"message\":\"test4\",\"date\":1329462576867},{\"authid\":2,\"author\":\"test\",\"message\":\"test\",\"date\":1329462730319},{\"authid\":2,\"author\":\"test\",\"message\":\"test\",\"date\":1329462730319},{\"authid\":2,\"author\":\"test\",\"message\":\"test\",\"date\":1329462789107},{\"authid\":2,\"author\":\"test\",\"message\":\"test\",\"date\":1329462789107},{\"authid\":2,\"author\":\"test\",\"message\":\"tes\",\"date\":1329462821805},{\"authid\":2,\"author\":\"test\",\"message\":\"tes\",\"date\":1329462821805},{\"authid\":2,\"author\":\"test\",\"message\":\"Привет\",\"date\":1329463920578},{\"authid\":2,\"author\":\"test\",\"message\":\"Привет\",\"date\":1329463920578},{\"authid\":2,\"author\":\"test\",\"message\":\"test123\",\"date\":1329464863332},{\"authid\":2,\"author\":\"test\",\"message\":\"test123\",\"date\":1329464863332},{\"authid\":2,\"author\":\"test\",\"message\":\"test\",\"date\":1329465850899},{\"authid\":2,\"author\":\"test\",\"message\":\"test\",\"date\":1329465850899},{\"authid\":2,\"author\":\"test\",\"message\":\"ghbdtn\",\"date\":1329465948303},{\"authid\":2,\"author\":\"test\",\"message\":\"ghbdtn\",\"date\":1329465948303},{\"authid\":2,\"author\":\"test\",\"message\":\"Ghbdtn\",\"date\":1329468618772},{\"authid\":2,\"author\":\"test\",\"message\":\"asdl;k;l\",\"date\":1329468667264},{\"authid\":4,\"author\":\"test2\",\"message\":\"123\",\"date\":1329468688502},{\"authid\":4,\"author\":\"test2\",\"message\":\"111\",\"date\":1329468807998},{\"authid\":4,\"author\":\"test2\",\"message\":\"private\",\"date\":1329468819766},{\"authid\":2,\"author\":\"test\",\"message\":\"124341234\",\"date\":1329472290805},{\"authid\":2,\"author\":\"test\",\"message\":\"124341234\",\"date\":1329472290805},{\"authid\":2,\"author\":\"test\",\"message\":\"qwerqwer\",\"date\":1329472653008},{\"authid\":2,\"author\":\"test\",\"message\":\"qwerqwer\",\"date\":1329472653008},{\"authid\":2,\"author\":\"test\",\"message\":\"привет тесту\",\"date\":1329472835511},{\"authid\":2,\"author\":\"test\",\"message\":\"привет тесту\",\"date\":1329472835511},{\"authid\":2,\"author\":\"test\",\"message\":\"привет себе\",\"date\":1329472846981},{\"authid\":2,\"author\":\"test\",\"message\":\"второй привет\",\"date\":1329472871351},{\"authid\":3,\"author\":\"test1\",\"message\":\"опа\",\"date\":1329472920562},{\"authid\":2,\"author\":\"test\",\"message\":\"привет\",\"date\":1329473807945}],\"lastPing\":1338638778021,\"challenge\":{\"3\":\"test1\"},\"duels\":{\"3\":{\"user\":\"test1\",\"accept\":false}},\"inventory\":{\"0\":{\"id\":5,\"gid\":0,\"title\":\"Щит Бабуина\",\"body\":\"{\\\"src\\\":\\\"05.png\\\"}\",\"image\":\"\",\"unit\":0,\"sellcost\":0,\"buycost\":0},\"1\":{\"id\":4,\"gid\":0,\"title\":\"Меч Кладенец\",\"body\":\"{\\\"src\\\":\\\"04.png\\\"}\",\"image\":\"\",\"unit\":0,\"sellcost\":0,\"buycost\":0},\"3\":{\"id\":2,\"gid\":0,\"title\":\"Сладкий Сухарик\",\"body\":\"{\\\"src\\\":\\\"02.png\\\"}\",\"image\":\"\",\"unit\":0,\"sellcost\":0,\"buycost\":0}},\"inventorysize\":10}',0,0,0,'0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00',0),(3,1,'','{\"tasks\":{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"title\":\"Начало игры\",\"instruction\":\"b5084324-b6a5-462c-8eed-780e9e18f1fd\",\"closed\":true}},\"personage\":{\"lvl\":\"1\",\"race\":\"angels\"},\"subscription\":{\"0\":\"Main Channel\"},\"personageId\":3,\"lastPing\":1334894464717,\"messages\":[{\"authid\":1,\"author\":\"blacksmith\",\"message\":\"dsfa\",\"date\":1333712125072},{\"authid\":1,\"author\":\"blacksmith\",\"message\":\"dsfa\",\"date\":1333712125072}],\"challenge\":{},\"inventory\":{\"0\":{\"id\":5,\"gid\":0,\"title\":\"Щит Бабуина\",\"body\":\"{\\\"src\\\":\\\"05.png\\\"}\",\"image\":\"\",\"unit\":0,\"sellcost\":0,\"buycost\":0},\"1\":{\"id\":4,\"gid\":0,\"title\":\"Меч Кладенец\",\"body\":\"{\\\"src\\\":\\\"04.png\\\"}\",\"image\":\"\",\"unit\":0,\"sellcost\":0,\"buycost\":0},\"2\":{\"id\":3,\"gid\":0,\"title\":\"Плюшка Латунная\",\"body\":\"{\\\"src\\\":\\\"03.png\\\"}\",\"image\":\"\",\"unit\":0,\"sellcost\":0,\"buycost\":0},\"3\":{\"id\":2,\"gid\":0,\"title\":\"Сладкий Сухарик\",\"body\":\"{\\\"src\\\":\\\"02.png\\\"}\",\"image\":\"\",\"unit\":0,\"sellcost\":0,\"buycost\":0},\"4\":{\"id\":1,\"gid\":0,\"title\":\"Пенный Пивас\",\"body\":\"{\\\"src\\\":\\\"01.png\\\"}\",\"image\":\"\",\"unit\":0,\"sellcost\":0,\"buycost\":0}},\"inventorysize\":10}',0,0,0,'0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00',0),(4,4,'','{\"tasks\":{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"title\":\"Начало игры\",\"instruction\":\"b5084324-b6a5-462c-8eed-780e9e18f1fd\",\"closed\":true}},\"personage\":{\"lvl\":\"1\",\"race\":\"angels\"},\"subscription\":{\"0\":\"Main Channel\"},\"lastPing\":1329471743538,\"messages\":[{\"authid\":2,\"author\":\"test\",\"message\":\"Ghbdtn\",\"date\":1329468618772},{\"authid\":2,\"author\":\"test\",\"message\":\"asdl;k;l\",\"date\":1329468667264},{\"authid\":4,\"author\":\"test2\",\"message\":\"123\",\"date\":1329468688502},{\"authid\":4,\"author\":\"test2\",\"message\":\"111\",\"date\":1329468807998},{\"authid\":4,\"author\":\"test2\",\"message\":\"private\",\"date\":1329468819766}],\"personageId\":4}',0,0,0,'0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00',0),(5,3,'','{\"tasks\":{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"title\":\"Начало игры\",\"instruction\":\"b5084324-b6a5-462c-8eed-780e9e18f1fd\",\"closed\":true}},\"personage\":{\"lvl\":\"1\",\"race\":\"angels\"},\"subscription\":{\"0\":\"Main Channel\"},\"lastPing\":1329814024191,\"personageId\":5,\"messages\":[{\"authid\":3,\"author\":\"test1\",\"message\":\"asdfadsfwqer\",\"date\":1329472671368},{\"authid\":3,\"author\":\"test1\",\"message\":\"asdfadsfwqer\",\"date\":1329472671368},{\"authid\":2,\"author\":\"test\",\"message\":\"привет себе\",\"date\":1329472846981},{\"authid\":2,\"author\":\"test\",\"message\":\"второй привет\",\"date\":1329472871351},{\"authid\":3,\"author\":\"test1\",\"message\":\"опа\",\"date\":1329472920562},{\"authid\":3,\"author\":\"test1\",\"message\":\"sdfdsf\",\"date\":1329473019017},{\"authid\":3,\"author\":\"test1\",\"message\":\"sdfdsf\",\"date\":1329473019017},{\"authid\":3,\"author\":\"test1\",\"message\":\"asdfdasf\",\"date\":1329473150532},{\"authid\":3,\"author\":\"test1\",\"message\":\"asdfdasf\",\"date\":1329473150532},{\"authid\":2,\"author\":\"test\",\"message\":\"привет\",\"date\":1329473807945}],\"challenge\":{\"2\":\"test\"},\"duels\":{\"2\":{\"user\":\"test\"}}}',0,0,0,'0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00',0),(6,1,'','{\"tasks\":{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"title\":\"Начало игры\",\"instruction\":\"ba409f75-cfff-4d6d-9e7f-7f2c1f1d4727\"}},\"personage\":{\"lvl\":0,\"race\":0}}',0,0,0,'0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00',0),(7,1,'','{\"tasks\":{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"title\":\"Начало игры\",\"instruction\":\"ba409f75-cfff-4d6d-9e7f-7f2c1f1d4727\"}},\"personage\":{\"lvl\":0,\"race\":0}}',0,0,0,'0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00',0),(8,6,'','{\"tasks\":{\"b173bd0d-a83a-47cf-a60b-7157e12a11db\":{\"title\":\"Начало игры\",\"instruction\":\"ba409f75-cfff-4d6d-9e7f-7f2c1f1d4727\"}},\"personage\":{\"lvl\":0,\"race\":0},\"personageId\":8}',0,0,0,'0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00','0.00',0);
/*!40000 ALTER TABLE `personage` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `personage_types`
--

DROP TABLE IF EXISTS `personage_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `personage_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Personage Type Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Personage Type Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Personage Types';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `personage_types`
--

LOCK TABLES `personage_types` WRITE;
/*!40000 ALTER TABLE `personage_types` DISABLE KEYS */;
INSERT INTO `personage_types` VALUES (1,'qwe','qwe');
/*!40000 ALTER TABLE `personage_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_messages`
--

DROP TABLE IF EXISTS `quest_messages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_messages` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Quest Message Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Quest Message Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Quest Messages';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_messages`
--

LOCK TABLES `quest_messages` WRITE;
/*!40000 ALTER TABLE `quest_messages` DISABLE KEYS */;
INSERT INTO `quest_messages` VALUES (1,'qwe','qwe');
/*!40000 ALTER TABLE `quest_messages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ratio`
--

DROP TABLE IF EXISTS `ratio`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ratio` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL DEFAULT '0',
  `login` varchar(255) NOT NULL DEFAULT '' COMMENT 'News Title',
  `ratio` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ratio`
--

LOCK TABLES `ratio` WRITE;
/*!40000 ALTER TABLE `ratio` DISABLE KEYS */;
/*!40000 ALTER TABLE `ratio` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `server_methods`
--

DROP TABLE IF EXISTS `server_methods`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `server_methods` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Method Name',
  `body` text NOT NULL COMMENT 'Method Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `server_methods`
--

LOCK TABLES `server_methods` WRITE;
/*!40000 ALTER TABLE `server_methods` DISABLE KEYS */;
INSERT INTO `server_methods` VALUES (1,'init','function(resp, params) {\n	components.getView(resp, {\"view\": \"login\"});\n	components.getView(resp, {\"view\": \"register\"});\n	components.getMethod(resp, {\"method\": \"clearCanvas\"});\n	components.getMethod(resp, {\"method\": \"carousel\"});\n	components.getMethod(resp, {\"method\": \"showModal\"});\n	components.getMethod(resp, {\"method\": \"hideWindow\"});\n	components.getMethod(resp, {\"method\": \"showWindow\"});\n	components.getMethod(resp, {\"method\": \"stopCarousel\"});\n	components.getMethod(resp, {\"method\": \"startCarousel\"});\n	resp.push({\n		\"cmd\" : \"startCarousel\",\n		\"params\" : \"\"\n	});\n};'),(2,'login','function(resp, params) {\n	var checkSql = \'SELECT * FROM auth_identity WHERE login=\\\'\'\n			+ conn.escapeSync(params.login) + \'\\\' AND password=\\\'\'\n			+ hashlib.sha1(params.password) + \'\\\' LIMIT 1\';\n	var res = conn.querySync(checkSql);\n	if (res != false) {\n		var rows = res.fetchAllSync();\n		if (rows.length > 0) {\n			params.session.identity = rows[0];\n			var checkSql = \'SELECT * FROM personage WHERE user_id=\' + params.session.identity.id + \' LIMIT 1\'; \n			var res = conn.querySync(checkSql);\n			if (res != false) {\n				if (rows.length > 0) {\n					contexts[params.session.identity.id] = params.session.context = JSON.parse(rows[0][\'body\']);\n					params.session.personageId = rows[0][\'id\'];\n				}\n			}\n            \n			resp.push({\n				\"cmd\" : \"hideWindow\",\n				\"params\" : \"login-window\"\n			});\n			resp.push({\n				\"cmd\" : \"stopCarousel\",\n				\"params\" : \"\"\n			});\n			components.getMethod(resp, {\"method\": \"selectRace\"});\n			components.getMethod(resp, {\"method\": \"startSelectRace\"});\n			components.getMethod(resp, {\"method\": \"stopSelectRace\"});\n			components.getMethod(resp, {\"method\": \"alert\"});\n			resp.push({\n				\"cmd\" : \"clearCanvas\",\n				\"params\" : \"\"\n			});\n		} else\n		{\n			// Wrong auth\n			resp.push({\n				\"cmd\" : \"showModal\",\n				\"params\" : {\"header\" : \"Wrong login or password\", \"body\" : \"Try another\"}\n			});\n		}\n	} else {\n		util.puts(\'no res in login\');\n	}\n};'),(3,'register','function(resp, params) {\n	// TODO check fields\n	var insertSql = \'INSERT INTO auth_identity (login, password, email, first_name, second_name) VALUES (\\\'\'\n			+ conn.escapeSync(params.login)\n			+ \'\\\', \\\'\'\n			+ hashlib.sha1(params.password)\n			+ \'\\\',\\\'\'\n			//+ conn.escapeSync(params.email)\n			+ \'\\\',\\\'\'\n			//+ conn.escapeSync(params.first_name)\n			+ \'\\\',\\\'\'\n			//+ conn.escapeSync(params.second_name) \n			+ \'\\\')\';\n	var res = conn.querySync(insertSql);\n	if (res != false) {\n		var checkSql = \'SELECT * FROM auth_identity WHERE login=\\\'\'\n			+ conn.escapeSync(params.login) + \'\\\' AND password=\\\'\'\n			+ hashlib.sha1(params.password) + \'\\\' LIMIT 1\';\n		var res = conn.querySync(checkSql);\n		if (res != false) {\n			var rows = res.fetchAllSync();\n				params.session.identity = rows[0];\n				resp.push({\n					\"cmd\" : \"hideWindow\",\n					\"params\" : \"register-window\"\n				});\n				resp.push({\n					\"cmd\" : \"stopCarousel\",\n					\"params\" : \"\"\n				});\n				components.getView(resp, {\"view\": \"avatar\"});\n				components.getView(resp, {\"view\": \"best\"});\n				components.getView(resp, {\"view\": \"chat-window\"});\n				components.getView(resp, {\"view\": \"duel\"});\n				components.getView(resp, {\"view\": \"info\"});\n				components.getView(resp, {\"view\": \"map\"});\n				components.getView(resp, {\"view\": \"mentor\"});\n				components.getView(resp, {\"view\": \"premium\"});\n				components.getView(resp, {\"view\": \"ratio\"});\n				components.getView(resp, {\"view\": \"shop\"});\n				resp.push({\n					\"cmd\" : \"clearCanvas\",\n					\"params\" : \"\"\n				});\n		} else {\n			util.puts(\'no res in login\');\n		}		\n	} else\n	{\n		// Wrong\n		resp.push({\n			\"cmd\" : \"showModal\",\n			\"params\" : {\"header\" : \"Server error\", \"body\" : \"Try another\"}\n		});\n	}\n};');
/*!40000 ALTER TABLE `server_methods` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sounds`
--

DROP TABLE IF EXISTS `sounds`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sounds` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Sound Item Title',
  `body` varchar(255) NOT NULL DEFAULT '' COMMENT 'Sound Item Body',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Game Events';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sounds`
--

LOCK TABLES `sounds` WRITE;
/*!40000 ALTER TABLE `sounds` DISABLE KEYS */;
INSERT INTO `sounds` VALUES (1,'axzc','23e2rewr');
/*!40000 ALTER TABLE `sounds` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `step_conditions`
--

DROP TABLE IF EXISTS `step_conditions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `step_conditions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `step_id` int(11) NOT NULL,
  `param` varchar(64) NOT NULL DEFAULT '',
  `condition` varchar(10) NOT NULL DEFAULT '',
  `val` mediumtext NOT NULL,
  `reg_in` varchar(1) NOT NULL DEFAULT '',
  `reg_out` varchar(1) NOT NULL DEFAULT '',
  `cmd` varchar(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `step_conditions`
--

LOCK TABLES `step_conditions` WRITE;
/*!40000 ALTER TABLE `step_conditions` DISABLE KEYS */;
/*!40000 ALTER TABLE `step_conditions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `step_instructions`
--

DROP TABLE IF EXISTS `step_instructions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `step_instructions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `step_id` int(11) NOT NULL,
  `client_cmd` varchar(64) NOT NULL DEFAULT '',
  `client_param` mediumtext NOT NULL,
  `reg_in` varchar(1) NOT NULL DEFAULT '',
  `reg_out` varchar(1) NOT NULL DEFAULT '',
  `cmd` varchar(64) NOT NULL DEFAULT '',
  `param` varchar(64) NOT NULL DEFAULT '',
  `val` mediumtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `step_instructions`
--

LOCK TABLES `step_instructions` WRITE;
/*!40000 ALTER TABLE `step_instructions` DISABLE KEYS */;
/*!40000 ALTER TABLE `step_instructions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `task_step`
--

DROP TABLE IF EXISTS `task_step`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `task_step` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `task_id` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `task_step`
--

LOCK TABLES `task_step` WRITE;
/*!40000 ALTER TABLE `task_step` DISABLE KEYS */;
/*!40000 ALTER TABLE `task_step` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tasks`
--

DROP TABLE IF EXISTS `tasks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tasks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` mediumtext NOT NULL,
  `last_step` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tasks`
--

LOCK TABLES `tasks` WRITE;
/*!40000 ALTER TABLE `tasks` DISABLE KEYS */;
/*!40000 ALTER TABLE `tasks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `trigger_conditions`
--

DROP TABLE IF EXISTS `trigger_conditions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `trigger_conditions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `trigger_id` int(11) NOT NULL,
  `param` varchar(64) NOT NULL DEFAULT '',
  `condition` varchar(10) NOT NULL DEFAULT '',
  `val` mediumtext NOT NULL,
  `reg_in` varchar(1) NOT NULL DEFAULT '',
  `reg_out` varchar(1) NOT NULL DEFAULT '',
  `cmd` varchar(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `trigger_conditions`
--

LOCK TABLES `trigger_conditions` WRITE;
/*!40000 ALTER TABLE `trigger_conditions` DISABLE KEYS */;
/*!40000 ALTER TABLE `trigger_conditions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `triggers`
--

DROP TABLE IF EXISTS `triggers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `triggers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` mediumtext NOT NULL,
  `task_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `triggers`
--

LOCK TABLES `triggers` WRITE;
/*!40000 ALTER TABLE `triggers` DISABLE KEYS */;
/*!40000 ALTER TABLE `triggers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_params`
--

DROP TABLE IF EXISTS `user_params`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_params` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL DEFAULT '0',
  `param` varchar(64) NOT NULL DEFAULT '',
  `context_id` int(11) NOT NULL DEFAULT '0',
  `val` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_params`
--

LOCK TABLES `user_params` WRITE;
/*!40000 ALTER TABLE `user_params` DISABLE KEYS */;
/*!40000 ALTER TABLE `user_params` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_steps`
--

DROP TABLE IF EXISTS `user_steps`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_steps` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `step_id` int(11) NOT NULL DEFAULT '0',
  `user_id` int(11) NOT NULL DEFAULT '0',
  `task_id` int(11) NOT NULL DEFAULT '0',
  `closed` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_steps`
--

LOCK TABLES `user_steps` WRITE;
/*!40000 ALTER TABLE `user_steps` DISABLE KEYS */;
/*!40000 ALTER TABLE `user_steps` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_tasks`
--

DROP TABLE IF EXISTS `user_tasks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_tasks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `task_id` int(11) NOT NULL,
  `step_id` int(11) NOT NULL DEFAULT '0',
  `last_step` int(11) NOT NULL DEFAULT '0',
  `closed` int(11) NOT NULL DEFAULT '0',
  `context_id` int(11) NOT NULL DEFAULT '0',
  `b` mediumtext NOT NULL,
  `c` mediumtext NOT NULL,
  `d` mediumtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_tasks`
--

LOCK TABLES `user_tasks` WRITE;
/*!40000 ALTER TABLE `user_tasks` DISABLE KEYS */;
INSERT INTO `user_tasks` VALUES (1,0,0,0,0,0,0,'[\"personage\"]','[]','[]');
/*!40000 ALTER TABLE `user_tasks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'fallen'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-06-02 20:06:29
