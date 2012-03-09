----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    22:22:47 03/09/2012 
-- Design Name: 
-- Module Name:    toplevel - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity toplevel is
    Port ( address : in  STD_LOGIC_VECTOR (23 downto 0);
           addressstrobe : in  STD_LOGIC;
           serialout : out  STD_LOGIC_VECTOR (2 downto 0);
           clock : in  STD_LOGIC);
end toplevel;

architecture Behavioral of toplevel is
	
	COMPONENT shiftreg
	PORT(
		data : IN std_logic_vector(7 downto 0);
		reset : IN std_logic;
		clock : IN std_logic;          
		output : OUT std_logic
		);
	END COMPONENT;

begin
	
	Inst_shiftregh: shiftreg PORT MAP(
		data => address(23 downto 16),
		reset => addressstrobe,
		clock => clock,
		output => serialout(2)
	);
	
	Inst_shiftregm: shiftreg PORT MAP(
		data => address(15 downto 8),
		reset => addressstrobe,
		clock => clock,
		output => serialout(1)
	);
	
	Inst_shiftreg: shiftreg PORT MAP(
		data => address(7 downto 0),
		reset => addressstrobe,
		clock => clock,
		output => serialout(0)
	);

end Behavioral;

