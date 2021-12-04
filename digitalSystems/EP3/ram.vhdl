-------------------------------------------------------
--! @file ram.vhdl
--! @brief Implementação de RAM genérica em VHDL para EP3 da disciplina de Sistemas Digitais I
--! @author Igor Pontes Tresolavy (tresolavy@usp.br)
--! @date 2021-11-30
-------------------------------------------------------

library IEEE;
use IEEE.numeric_bit.all;

entity ram is
    generic (
        addressSize : natural := 4;
        wordSize    : natural := 8
    );
    port (
        ck, wr  : in bit;
        addr    : in bit_vector(addressSize-1 downto 0);
        data_i  : in bit_vector(wordSize-1 downto 0);
        data_o  : out bit_vector(wordSize-1 downto 0)    
    );
end entity ram;

architecture random_access_memory of ram is 

        type mem_type is array (0 to 2**addressSize) of bit_vector(wordSize-1 downto 0);
        signal mem : mem_type;

    begin

        memory_write: process(ck)
            begin
                if (ck'event and ck = '1') then
                    if (wr = '1') then
                        mem(to_integer(unsigned(addr))) <= data_i;
                    end if;
                end if;
        end process;

        data_o <= mem(to_integer(unsigned(addr)));

end architecture random_access_memory;