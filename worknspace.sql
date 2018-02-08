-- phpMyAdmin SQL Dump
-- version 4.6.4
-- https://www.phpmyadmin.net/
--
-- Client :  127.0.0.1
-- Généré le :  Jeu 08 Février 2018 à 15:16
-- Version du serveur :  5.7.14
-- Version de PHP :  5.6.25

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données :  `worknspace`
--

-- --------------------------------------------------------

--
-- Structure de la table `booking`
--

CREATE TABLE `booking` (
  `id_booking` int(11) NOT NULL,
  `date_begin_booking` datetime DEFAULT NULL,
  `date_finish_booking` datetime DEFAULT NULL,
  `id_location` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `booking_equipment`
--

CREATE TABLE `booking_equipment` (
  `id_booking_equipment` int(11) NOT NULL,
  `date_rent` datetime DEFAULT NULL,
  `date_return` datetime DEFAULT NULL,
  `id_equipment` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `customer`
--

CREATE TABLE `customer` (
  `id_customer` int(11) NOT NULL,
  `name_customer` varchar(80) NOT NULL,
  `surname_customer` varchar(80) NOT NULL,
  `phone_number` char(10) NOT NULL,
  `email` varchar(255) NOT NULL,
  `code_customer` char(10) NOT NULL,
  `id_subscription` int(11) DEFAULT NULL,
  `id_location` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `equipment`
--

CREATE TABLE `equipment` (
  `id_equipment` int(11) NOT NULL,
  `name_equipment` varchar(100) NOT NULL,
  `quantity` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `history`
--

CREATE TABLE `history` (
  `id_history` int(11) NOT NULL,
  `date_entry` datetime DEFAULT NULL,
  `date_sorty` datetime DEFAULT NULL,
  `id_customer` int(11) DEFAULT NULL,
  `id_location` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `location`
--

CREATE TABLE `location` (
  `id_location` int(11) NOT NULL,
  `town` varchar(50) NOT NULL,
  `address` int(255) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `location_equipment`
--

CREATE TABLE `location_equipment` (
  `id_location` int(11) NOT NULL,
  `id_equipment` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `location_schedule`
--

CREATE TABLE `location_schedule` (
  `id_location` int(11) NOT NULL,
  `id_schedule` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `room`
--

CREATE TABLE `room` (
  `id_room` int(11) NOT NULL,
  `type_room` varchar(100) NOT NULL,
  `number_places` int(11) NOT NULL,
  `id_town` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `schedule`
--

CREATE TABLE `schedule` (
  `id_schedule` int(11) NOT NULL,
  `day` varchar(8) NOT NULL,
  `begin_schedule` time NOT NULL,
  `end_schedule` time NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `staff`
--

CREATE TABLE `staff` (
  `id_staff` int(11) NOT NULL,
  `name_staff` varchar(80) NOT NULL,
  `surname_staff` varchar(80) NOT NULL,
  `hierarchy` int(11) NOT NULL,
  `id_location` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `staff_ticket`
--

CREATE TABLE `staff_ticket` (
  `id_staff` int(11) DEFAULT NULL,
  `id_ticket` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `subscription`
--

CREATE TABLE `subscription` (
  `id_subscription` int(11) NOT NULL,
  `type_subscription` int(11) NOT NULL,
  `date_begin_subscription` datetime NOT NULL,
  `date_finish_subscription` datetime NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `ticket`
--

CREATE TABLE `ticket` (
  `id_ticket` int(11) NOT NULL,
  `subject` varchar(150) NOT NULL,
  `description` longtext NOT NULL,
  `id_equipment` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Index pour les tables exportées
--

--
-- Index pour la table `booking`
--
ALTER TABLE `booking`
  ADD PRIMARY KEY (`id_booking`);

--
-- Index pour la table `booking_equipment`
--
ALTER TABLE `booking_equipment`
  ADD PRIMARY KEY (`id_booking_equipment`);

--
-- Index pour la table `customer`
--
ALTER TABLE `customer`
  ADD PRIMARY KEY (`id_customer`);

--
-- Index pour la table `equipment`
--
ALTER TABLE `equipment`
  ADD PRIMARY KEY (`id_equipment`);

--
-- Index pour la table `history`
--
ALTER TABLE `history`
  ADD PRIMARY KEY (`id_history`);

--
-- Index pour la table `location`
--
ALTER TABLE `location`
  ADD PRIMARY KEY (`id_location`);

--
-- Index pour la table `location_equipment`
--
ALTER TABLE `location_equipment`
  ADD PRIMARY KEY (`id_location`,`id_equipment`);

--
-- Index pour la table `location_schedule`
--
ALTER TABLE `location_schedule`
  ADD PRIMARY KEY (`id_location`,`id_schedule`);

--
-- Index pour la table `room`
--
ALTER TABLE `room`
  ADD PRIMARY KEY (`id_room`);

--
-- Index pour la table `schedule`
--
ALTER TABLE `schedule`
  ADD PRIMARY KEY (`id_schedule`);

--
-- Index pour la table `staff`
--
ALTER TABLE `staff`
  ADD PRIMARY KEY (`id_staff`);

--
-- Index pour la table `subscription`
--
ALTER TABLE `subscription`
  ADD PRIMARY KEY (`id_subscription`);

--
-- Index pour la table `ticket`
--
ALTER TABLE `ticket`
  ADD PRIMARY KEY (`id_ticket`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
