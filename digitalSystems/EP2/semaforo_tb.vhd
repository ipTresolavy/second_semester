-------------------------------------------------------
--! @file semaforo_tb.vhd
--! @brief Testbench para o controlador de semáforo
--! @author Edson S. Gomi (gomi@usp.br)
--! @date 2021-10-26
-------------------------------------------------------

--  A testbench has no ports.
entity semaforo_tb is
end entity semaforo_tb;

architecture testbench of semaforo_tb is

  --  Declaration of the component to be tested.  
  component semaforo
    port(  
      CLOCK : in bit;
      TA : in bit;
      TB : in bit;
      LA : out bit_vector(1 downto 0);
      LB : out bit_vector(1 downto 0)
      );
  end component semaforo;
  
  -- Declaration of signals
  signal clock : bit;
  signal ta, tb : bit;
  signal la, lb : bit_vector(1 downto 0);

begin
  -- Component instantiation
  -- DUT = Device Under Test 
  DUT: entity work.semaforo(fsm) port map (clock,ta,tb,la,lb);

  -- Clock generator
  clk: process is
  begin
    clock <= '0';
    wait for 0.5 ns;
    clock <= '1';
    wait for 0.5 ns;
  end process clk;  

  --  This process does the real job.
  stimulus_process: process is
    type pattern_type is record
      --  The inputs of the circuit.
      ta : bit;
      tb : bit;
      --  The expected outputs of the circuit.
      la : bit_vector(1 downto 0);
      lb : bit_vector(1 downto 0);
    end record;

    --  The patterns to apply.
    type pattern_array is array (natural range <>) of pattern_type;
    constant patterns : pattern_array :=
      (
        ('1','0',"00","01"),
        ('0','0',"11","01"),
        ('1','1',"01","00"),
        ('0','1',"01","00"),
        ('0','0',"01","11"),
        ('1','1',"00","01"),
        ('1','1',"00","01"));

  begin
    --  Check each pattern.
    for k in patterns'range loop

      --  Set the inputs.
      ta <= patterns(k).ta;
      tb <= patterns(k).tb;

      --  Wait for the results.
      wait for 1 ns;
      
      --  Check the outputs.
      assert la = patterns(k).la
        report "bad LA" severity error;

      assert lb = patterns(k).lb
        report "bad LB" severity error;
    end loop;
    
    assert false report "end of test" severity note;
    
  --  Wait forever; this will finish the simulation.
    wait;
  end process;
end architecture testbench;

  

