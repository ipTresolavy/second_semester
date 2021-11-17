-------------------------------------------------------
--! @file alu.vhdl
--! @brief Unidade lógico-aritmética para EP2 da disciplina de Sistemas Digitais I
--! @author Igor Pontes Tresolavy (tresolavy@usp.br)
--! @date 2021-10-30
-------------------------------------------------------

library IEEE;
use IEEE.numeric_bit.all;

entity alu is
    generic (
        size    : natural := 8
    );
    port (
        A, B    : in  bit_vector(size-1 downto 0); -- inputs
        F       : out bit_vector(size-1 downto 0); -- output
        S       : in  bit_vector(2 downto 0); -- op selection
        Z       : out bit; -- zero flag
        Ov      : out bit; -- overflow flag
        Co      : out bit -- carry out
    );
end entity alu;

architecture operation of alu is

    -- signals used in arithmetic operations
    signal half_add : bit_vector(size-1 downto 0);
    signal full_add : bit_vector(size-1 downto 0); -- op codes 001 and 100
    signal carrys   : bit_vector(size downto 0);

    -- signals used in logic operations
    signal and_op   : bit_vector(size-1 downto 0); -- op code 010
    signal or_op    : bit_vector(size-1 downto 0); -- op code 011
    signal not_op   : bit_vector(size-1 downto 0); -- op code 101
    signal inv_op   : bit_vector(size-1 downto 0); -- op code 110

    -- mask of zeroes for zero flag checking
    constant mask   : bit_vector(size-1 downto 0):=(others => '0');

    -- final output signal
    signal output_F : bit_vector(size-1 downto 0);

begin

    -- if subtracting, initial carry is 1 because of two's complement
    with S select
        carrys(0) <= '1' when "100",
                     '0' when others;

    -- addition and subtraction operations, op codes 001 and 100, respectively
    arth_operations: for i in size-1 downto 0 generate
        with S select
            half_add(i) <= A(i) XOR (not B(i)) when "100",
                           A(i) XOR B(i) when others; -- when S = "001"
                           
        full_add(i) <= half_add(i) XOR carrys(i);

        with S select -- propagates and generates
            carrys(i + 1) <= (A(i) and (not B(i))) or (half_add(i) and carrys(i)) when "100",
                             (A(i) and B(i)) or (half_add(i) and carrys(i)) when others; -- when S = "001"
    end generate arth_operations;

    lgc_operations: for i in size-1 downto 0 generate
        and_op(i) <= A(i) and B(i); -- AND operation, op code 010
        or_op(i) <= A(i) OR B(i);   -- OR operation, op code 011
        not_op(i) <= not A(i);      -- NOT operation, op code 101
        inv_op(i) <= A(size-1 - i); -- Inversion operation, op code 110
    end generate lgc_operations;

    with S select
        output_F <= A when "000",
                    full_add when "001",
                    and_op when "010",
                    or_op when "011",
                    full_add when "100",
                    not_op when "101",
                    inv_op when "110",
                    B when others;
    
    F <= output_F;
    
    Z <= '1' when (output_F = mask) else
         '0';
    
    Co <= carrys(size);
    Ov <= carrys(size) XOR carrys(size - 1);

end architecture operation;