
-- VHDL Instantiation Created from source file shiftreg.vhd -- 22:46:33 03/09/2012
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT shiftreg
	PORT(
		data : IN std_logic_vector(7 downto 0);
		reset : IN std_logic;
		clock : IN std_logic;          
		serialout : OUT std_logic
		);
	END COMPONENT;

	Inst_shiftreg: shiftreg PORT MAP(
		data => ,
		reset => ,
		clock => ,
		serialout => 
	);


