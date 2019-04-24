class SortExperiment
  def initialize(kind)
    @kind = kind
  end

  def execute
    %w(random asc desc not_unique).each do |filename|
      print("#{@kind} sort(#{filename} data): ")
      system("./#{@kind}_sort #{filename}.txt")
      puts
      system("time ./#{@kind}_sort #{filename}.txt")
    end
  end
end

%w(bubble quick).each { |kind| SortExperiment.new(kind).execute }
