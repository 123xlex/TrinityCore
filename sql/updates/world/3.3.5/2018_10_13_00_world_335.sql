-- 
DELETE FROM `creature` WHERE `guid` IN (90288,90289,90296,90377,90378,90379,90380,90426,90427,90428,90429);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`,`spawndist`, `MovementType`) VALUES
(90288, 5434, 1, 1, 1, 0, 0, -4485, -4938, -19, 1.941, 900, 7, 1),
(90289, 5434, 1, 1, 1, 0, 0, -4455, -5045, -25, 4.998, 900, 7, 1),
(90296, 5434, 1, 1, 1, 0, 0, -4445, -5139, -23, 1.902, 900, 7, 1),
(90377, 5434, 1, 1, 1, 0, 0, -4376, -5228, -46, 5.992, 900, 7, 1),
(90378, 5434, 1, 1, 1, 0, 0, -4296, -5249, -46, 6.164, 900, 7, 1),
(90379, 5434, 1, 1, 1, 0, 0, -4266, -5176, -24, 5.762, 900, 7, 1),
(90380, 5434, 1, 1, 1, 0, 0, -4244, -5197, -34, 5.517, 900, 6, 1),
(90426, 5434, 1, 1, 1, 0, 0, -3948, -5211, -49, 3.358, 900, 6, 1),
(90427, 5434, 1, 1, 1, 0, 0, -3878, -5190, -45, 2.532, 900, 6, 1),
(90428, 5434, 1, 1, 1, 0, 0, -3858, -5237, -49, 6.027, 900, 6, 1),
(90429, 5434, 1, 1, 1, 0, 0, -1080, -4016, -79, 3.049, 900, 9, 1);