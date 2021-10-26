LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
-- USE ieee.numeric_std.ALL;
 
ENTITY tb_up_down IS
END tb_up_down;
 
ARCHITECTURE behavior OF tb_up_down IS
 
-- Component Declaration for the Unit Under Test (UUT)
 
COMPONENT up_down_counter
PORT(
    clock : IN std_logic;
    reset : IN std_logic;
    up_down : IN std_logic;
    counter : OUT std_logic_vector(3 downto 0)
);
END COMPONENT;
 
--Inputs
signal clock : std_logic := '0';
signal reset : std_logic := '0';
signal up_down : std_logic := '0';
 
--Outputs
signal counter : std_logic_vector(3 downto 0);
 
-- Clock period definitions
constant clock_period : time := 20 ns;
 
BEGIN
 
-- Instantiate the Unit Under Test (UUT)
    uut: up_down_counter PORT MAP (
         clock => clock,
         reset => reset,
         up_down => up_down,
         counter => counter
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
    wait for 20 ns;
    reset <= '1';
    wait for 20 ns;
    reset <= '0';
    up_down <= '0';
    wait for 200 ns;
    up_down <= '1';
    wait;

    end process;
 
END;