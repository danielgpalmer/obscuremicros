----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    22:45:57 03/09/2012 
-- Design Name: 
-- Module Name:    shiftreg - Behavioral 
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

entity shiftreg is
    Port ( data : in  STD_LOGIC_VECTOR (7 downto 0);
           reset : in  STD_LOGIC;
           clock : in  STD_LOGIC;
           output : out  STD_LOGIC);
end shiftreg;

architecture Behavioral of shiftreg is

	signal serialout : STD_LOGIC;
	signal clear: STD_LOGIC;

begin

	output <= serialout when (reset = '0') else 'Z';

	process(reset)
	begin
		if(falling_edge(reset)) then
			clear <= '1';
		end if;
	end process;

	process(clock)	
		variable index : integer range 0 to 7 := 0;
	begin
		if (rising_edge(clock) and reset = '0') then
			if (clear = '1') then
				index := 0;
				clear <= '0';
			end if;
			serialout <= data(index);
			index := index + 1;
		end if;
	end process;

end Behavioral;

