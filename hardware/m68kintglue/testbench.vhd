--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   23:06:45 03/09/2012
-- Design Name:   
-- Module Name:   /home/daniel/coding/obscuremicros/hardware/m68kintglue/testbench.vhd
-- Project Name:  m68kintglue
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: toplevel
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY testbench IS
END testbench;
 
ARCHITECTURE behavior OF testbench IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT toplevel
    PORT(
         address : IN  std_logic_vector(23 downto 0);
         addressstrobe : IN  std_logic;
         serialout : OUT  std_logic_vector(2 downto 0);
         clock : IN  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal address : std_logic_vector(23 downto 0);
   signal addressstrobe : std_logic;
   signal clock : std_logic;

 	--Outputs
   signal serialout : std_logic_vector(2 downto 0);

   -- Clock period definitions
   constant clock_period : time := 10 ns;
 
BEGIN
 
	address <= "101010101111000011110000";
 
	-- Instantiate the Unit Under Test (UUT)
   uut: toplevel PORT MAP (
          address => address,
          addressstrobe => addressstrobe,
          serialout => serialout,
          clock => clock
        );

   -- Clock process definitions
   clock_process :process
   begin
		clock <= '0';
		wait for clock_period/2;
		clock <= '1';
		wait for clock_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      addressstrobe <= '1';
		wait for 10 ns;	
		addressstrobe <= '0';
      wait for clock_period*10;

      -- insert stimulus here 

      wait;
   end process;

END;
